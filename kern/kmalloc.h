#pragma once

#include <stdint.h>
#include <string.h>

#include "kern.h"

void kmalloc_init();
void *kmalloc(size_t size);
void *kmalloc_aligned(size_t size, size_t align);
void kfree(void *p);

static inline uintptr_t pagenum(uintptr_t pa) {
    return pa / PAGESIZE;
}

static inline uintptr_t pageaddr(uintptr_t pn) {
    return pn * PAGESIZE;
}
