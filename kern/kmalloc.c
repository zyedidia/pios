#include <stdbool.h>
#include <stdint.h>

#include "kern.h"
#include "kmalloc.h"
#include "vm.h"
#include "lib.h"

// allocates pages of size 4096
#define MIN_ORDER 12
#define MAX_ORDER LOG_MEMSIZE_PHYSICAL

typedef struct {
    bool free;
    unsigned order;
} phys_page_t;

typedef struct free_page {
    struct free_page* next;
    struct free_page* prev;
} free_page_t;

// free list for each type of order
static free_page_t* free_lists[MAX_ORDER + 1];

static void ll_free_insert(free_page_t* n, int order) {
    n->next = free_lists[order];
    n->prev = NULL;
    if (free_lists[order])
        free_lists[order]->prev = n;
    free_lists[order] = n;
}

static void ll_free_remove(free_page_t* n, int order) {
    if (n->next)
        n->next->prev = n->prev;
    if (n->prev)
        n->prev->next = n->next;
    else
        free_lists[order] = n->next;
}

static phys_page_t pages[MEMSIZE_PHYSICAL / PAGESIZE];

static uintptr_t pagenum(uintptr_t pa) {
    return pa / PAGESIZE;
}

static uintptr_t pageaddr(uintptr_t pn) {
    return pn * PAGESIZE;
}

// Checks if a page number is valid for a given order
static bool valid(uintptr_t pn, unsigned order) {
    return pageaddr(pn) % (1 << order) == 0;
}

// Returns the page number of the buddy of the page stored at pn. Returns -1 if
// the given pn is not valid
static uintptr_t get_buddy(uintptr_t pn) {
    phys_page_t p = pages[pn];
    if (p.order < MIN_ORDER || p.order > MAX_ORDER || !valid(pn, p.order)) {
        return -1;
    }

    size_t pa = pageaddr(pn);
    if (valid(pn, p.order + 1)) {
        return pagenum(pa + (1 << p.order));
    }
    return pagenum(pa - (1 << p.order));
}

static free_page_t* pn_to_free(uintptr_t pn) {
    return (free_page_t*) pa2ka(pageaddr(pn));
}

extern char _kheap_start;

// Initialize everything needed for kalloc
void init_kalloc() {
    // Iterate through all heap memory, mark as free, and coalesce blocks
    // together if possible
    uintptr_t heap_start = (uintptr_t) &_kheap_start;
    for (uintptr_t pa = heap_start; pa < MEMSIZE_PHYSICAL; pa += PAGESIZE) {
        uintptr_t pn = pagenum(pa);
        pages[pn].free = true;
        pages[pn].order = MIN_ORDER;

        unsigned order = pages[pn].order;
        while (valid(pn, order)) {
            uintptr_t bpn = get_buddy(pn); // buddy pn
            // We can coalesce backwards
            if (bpn < pn && pages[bpn].free == pages[pn].free && pages[bpn].order == pages[pn].order) {
                // Merge blocks
                pages[bpn].order++;
                order++;
                pages[pn].order = 0;
                pn = bpn;
            }
            break;
        }
    }

    // Now we set up the free lists by looping over each block and adding it to
    // the list
    uintptr_t pn = 0;
    while (pn < pagenum(MEMSIZE_PHYSICAL)) {
        phys_page_t page = pages[pn];
        assert(valid(pn, page.order));
        if (page.free) {
            ll_free_insert(pn_to_free(pn), page.order);
        }
        pn += pagenum(1 << page.order);
    }
}

// Allocate and returns a pointer to at least `sz` contiguous bytes of memory.
// Returns `NULL` if `sz == 0` or on failure.
void* kalloc(size_t sz) {
    if (sz == 0) {
        return NULL;
    }

    unsigned order = msb(sz - 1);
    if (order < MIN_ORDER) {
        order = MIN_ORDER;
    }

    bool has_mem = true;
    while (has_mem) {
        has_mem = false;
        // Find a block that is >= the requested order. If we can't find such a
        // block the allocation fails.
        for (unsigned i = MIN_ORDER; i <= MAX_ORDER; i++) {
            if (free_lists[i]) {
                // found a free page
                uintptr_t pa = ka2pa((uintptr_t) free_lists[i]);
                uintptr_t pn = pagenum(pa);
                assert(pages[pn].free);
                assert(pages[pn].order == i);
                if (order == i) {
                    // The page matches the order so we can return it directly
                    ll_free_remove(free_lists[i], i);
                    pages[pn].free = false;
                    // TODO (asan): mark page as allocated
                    return (void*) pa2ka(pa);
                } else if (i > order) {
                    // We found a block that is greater than the requested
                    // order so there are no blocks with the correct size. We
                    // can split this block and try again.
                    pages[pn].order = i - 1;
                    uintptr_t bpn = get_buddy(pn);
                    pages[bpn].order = i - 1;
                    pages[bpn].free = true;

                    // update free lists
                    ll_free_remove(free_lists[i], i);
                    ll_free_insert(pn_to_free(pn), i - 1);
                    ll_free_insert(pn_to_free(bpn), i - 1);

                    has_mem = true;
                    break;
                }
            }
        }
    }

    // allocation failed
    return NULL;
}

// Free a pointer previously returned by `kalloc`. Does nothing if `ptr ==
// NULL`.
void kfree(void* ptr) {
    if (!ptr || (char*) ptr < &_kheap_start) {
        return;
    }

    uintptr_t pa = ka2pa((uintptr_t) ptr);
    uintptr_t pn = pagenum(pa);

    if (pages[pn].free) {
        // page is already free
        return;
    }

    pages[pn].free = true;
    uintptr_t bpn = get_buddy(pn);
    unsigned order = pages[pn].order;
    // TODO (asan): mark page as free

    while (bpn != (uintptr_t) -1 && pages[bpn].free && pages[bpn].order == pages[pn].order) {
        // coalesce
        ll_free_remove(pn_to_free(bpn), pages[pn].order);

        if (valid(pn, pages[pn].order + 1)) {
            order = ++pages[pn].order;
            pages[bpn].order = 0;
            bpn = get_buddy(pn);
        } else if (valid(bpn, pages[pn].order + 1)) {
            pages[pn].order = 0;
            order = ++pages[bpn].order;
            pn = bpn;
            bpn = get_buddy(bpn);
        }
    }

    ll_free_insert(pn_to_free(pn), order);
}
