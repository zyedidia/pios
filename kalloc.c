#include <stdint.h>

#include "kalloc.h"

extern char __heap_start__;
uintptr_t heap = (uintptr_t) &__heap_start__;

static inline uintptr_t align(uintptr_t ptr, size_t align) {
    return ((~ptr) + 1) & (align - 1);
}

void* kalloc(size_t size) {
    heap += size;
    heap += align(heap, 16);
    return (void*) heap;
}

void kfree(void* p) {
    (void) p;
}
