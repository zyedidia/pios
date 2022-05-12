#include <stdbool.h>
#include <stdint.h>

#include "kern.h"
#include "kmalloc.h"

union align {
    double d;
    void* p;
    void (*fp)(void);
};

typedef union free_hdr { /* block header */
    struct {
        union free_hdr* ptr; /* next block if on free list */
        unsigned size;       /* size of this block */
    } s;
    union align x; /* force alignment of blocks */
} free_hdr_t;

extern char _kheap_start;
uintptr_t heap_start() {
    return (uintptr_t) &_kheap_start;
}

static uintptr_t _kheap_end = (uintptr_t) &_kheap_start;
uintptr_t heap_end() {
    return _kheap_end;
}

static inline uintptr_t align_off(uintptr_t ptr, size_t align) {
    return ((~ptr) + 1) & (align - 1);
}

static void* sbrk(size_t size, size_t align) {
    void* ret = (void*) _kheap_end;
    _kheap_end += size;
    _kheap_end += align_off(_kheap_end, __alignof__(free_hdr_t));
    return ret;
}

static free_hdr_t base;          /* empty list to get started */
static free_hdr_t* freep = NULL; /* start of free list */

static void kr_free(void* ap);

#define NALLOC 1024 /* minimum #units to request */
/* morecore: ask system for more memory */
static free_hdr_t* morecore(unsigned nu) {
    char* cp;
    free_hdr_t* up;
    if (nu < NALLOC)
        nu = NALLOC;
    cp = sbrk(nu * sizeof(free_hdr_t));
    if (cp == (char*) -1) /* no space at all */
        return NULL;
    up = (free_hdr_t*) cp;
    up->s.size = nu;
    kr_free((void*) (up + 1));
    return freep;
}

static void* kr_malloc(size_t nbytes) {
    free_hdr_t *p, *prevp;
    unsigned nunits;
    nunits = (nbytes + sizeof(free_hdr_t) - 1) / sizeof(free_hdr_t) + 1;
    if ((prevp = freep) == NULL) { /* no free list yet */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {   /* big enough */
            if (p->s.size == nunits) /* exactly */
                prevp->s.ptr = p->s.ptr;
            else { /* allocate tail end */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void*) (p + 1);
        }
        if (p == freep) /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL; /* none left */
    }
}

/* free: put block ap in free list */
static void kr_free(void* ap) {
    free_hdr_t *bp, *p;
    bp = (free_hdr_t*) ap - 1; /* point to block header */
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* freed block at start or end of arena */
    if (bp + bp->s.size == p->s.ptr) { /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp) { /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;
    freep = p;
}

#if (SANITIZE == 0)

void* kmalloc(size_t sz) {
    return kr_malloc(sz);
}

void kfree(void* p) {
    kr_free(p);
}

#else

typedef struct asan_hdr {
    size_t size;
    struct asan_hdr* next;
    struct asan_hdr* prev;
} asan_hdr_t;

static asan_hdr_t* alloc_list;

static void ll_insert(asan_hdr_t* n) {
    n->next = alloc_list;
    n->prev = NULL;
    if (alloc_list)
        alloc_list->prev = n;
    alloc_list = n;
}

static void ll_remove(asan_hdr_t* n) {
    if (n->next)
        n->next->prev = n->prev;
    if (n->prev)
        n->prev->next = n->next;
    else
        alloc_list = n->next;
}

static char* blk_start(asan_hdr_t* h) {
    return (char*) h + sizeof(asan_hdr_t);
}

static char* blk_end(asan_hdr_t* h) {
    return (char*) h + sizeof(asan_hdr_t) + h->size;
}

void* kmalloc(size_t sz) {
    asan_hdr_t* h = (asan_hdr_t*) kr_malloc(sz + sizeof(asan_hdr_t));
    h->size = sz;
    ll_insert(h);
    return (void*) blk_start(h);
}

void kfree(void* p) {
    asan_hdr_t* h = (asan_hdr_t*) ((char*) p - sizeof(asan_hdr_t));
    ll_remove(h);
    kr_free(h);
}

static bool asan = false;

static bool in_range(char* addr, char* start, char* end) {
    return addr >= start && addr < end;
}

static void asan_access(unsigned long addr, size_t sz, bool write) {
    if (!asan) {
        return;
    }

    extern char _ktext_start, _ktext_end;
    if (write && in_range((char*) addr, &_ktext_start, &_ktext_end)) {
        panic("write to code segment: 0x%lx\n", addr);
    }
    extern char _krodata_start, _krodata_end;
    if (write && in_range((char*) addr, &_krodata_start, &_krodata_end)) {
        panic("write to read-only data segment: 0x%lx\n", addr);
    }
    if (in_range((char*) addr, &_kheap_start, (char*) _kheap_end)) {
        asan_hdr_t* h = alloc_list;
        while (h) {
            if (in_range((char*) addr, blk_start(h), blk_end(h))) {
                return;
            }
            h = h->next;
        }
        panic("illegal heap memory access: 0x%lx\n", addr);
    }
}

void __asan_load1_noabort(unsigned long addr) {
    asan_access(addr, 1, false);
}
void __asan_load2_noabort(unsigned long addr) {
    asan_access(addr, 2, false);
}
void __asan_load4_noabort(unsigned long addr) {
    asan_access(addr, 4, false);
}
void __asan_load8_noabort(unsigned long addr) {
    asan_access(addr, 8, false);
}
void __asan_loadN_noabort(unsigned long addr, size_t sz) {
    asan_access(addr, sz, false);
}

void __asan_store1_noabort(unsigned long addr) {
    asan_access(addr, 1, true);
}
void __asan_store2_noabort(unsigned long addr) {
    asan_access(addr, 2, true);
}
void __asan_store4_noabort(unsigned long addr) {
    asan_access(addr, 4, true);
}
void __asan_store8_noabort(unsigned long addr) {
    asan_access(addr, 8, true);
}
void __asan_storeN_noabort(unsigned long addr, size_t sz) {
    asan_access(addr, sz, true);
}

void __asan_handle_no_return() {}
void __asan_before_dynamic_init(const char* module_name) {}
void __asan_after_dynamic_init() {}

void asan_enable() {
    /* asan = true; */
}

#endif

void* kmalloc_aligned(size_t sz, size_t align) {
    uintptr_t x = (uintptr_t) kmalloc(sz + align);
    return (void*) (x + align_off(x, align));
}
