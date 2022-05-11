#pragma once

#include <string.h>

void asan_enable();
void* asan_kmalloc(size_t sz);
void asan_kfree(void* p);
