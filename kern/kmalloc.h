#pragma once

#include <string.h>
#include <stdint.h>

void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size, size_t align);
void kfree(void* p);

uintptr_t heap_start();
uintptr_t heap_end();
