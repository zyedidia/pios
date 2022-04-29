#include <stdint.h>

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

extern char __heap_start__;
uintptr_t heap = (uintptr_t) &__heap_start__;

static inline uintptr_t align(uintptr_t ptr, size_t align) {
    return ((~ptr) + 1) & (align - 1);
}

static void* sbrk(size_t size) {
    heap += size;
    heap += align(heap, __alignof__(free_hdr_t));
    return (void*) heap;
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

void* kmalloc(size_t sz) {
    return kr_malloc(sz);
}

void kfree(void* p) {
    kr_free(p);
}
