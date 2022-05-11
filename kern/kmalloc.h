#pragma once

#include <string.h>
#include <stdint.h>

extern uintptr_t heap_end;

void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size, size_t align);
void kfree(void* p);
