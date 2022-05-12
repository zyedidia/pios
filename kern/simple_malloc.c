#include <stdint.h>
#include "simple_malloc.h"
#include "vm.h"

// NOTE: THESE ARE KAs!
extern char _kheap_start;
static uintptr_t heap_end = (uintptr_t) &_kheap_start;

static inline uintptr_t align_off(uintptr_t ptr, uint32_t align) {
    return ((~ptr) + 1) & (align - 1);
}

void *smalloc_aligned(uint32_t size, uint32_t alignment) {
    heap_end += align_off(heap_end, alignment);
    uintptr_t pa = ka2pa(heap_end);
    heap_end += size;
    return pa;
}
