#pragma once

#include <string.h>

void asan_enable();
void asan_mark_memory(uintptr_t pa, size_t sz, bool alloc);
