#pragma once

static inline void dev_barrier() {
    asm volatile("mcr p15, 0, r0, c7, c10, 4");
}
