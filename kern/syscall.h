#pragma once

#include <stdint.h>

enum {
    SYSCALL_EXIT = 0,
    SYSCALL_ALLOC_PAGE,
    SYSCALL_DEALLOC_PAGE,
    SYSCALL_VM_MAP,
};

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
} user_regs_t;
