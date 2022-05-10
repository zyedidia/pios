#pragma once

#include <string.h>

void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size, size_t align);
void kfree(void* p);
