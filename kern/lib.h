#pragma once

static inline int msb(unsigned x) {
    return x ? sizeof(x) * 8 - __builtin_clz(x) : 0;
}
