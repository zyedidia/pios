#pragma once

#include <stdint.h>

#include "syscall.h"

static inline uintptr_t syscall_0(int sysno) {
    register uintptr_t r0 asm("r0") = sysno;
    asm volatile("swi 0" : "+r"(r0) : : "memory");
    return r0;
}
static inline uintptr_t syscall_1(int sysno, uintptr_t arg0) {
    register uintptr_t r0 asm("r0") = sysno;
    register uintptr_t r1 asm("r1") = arg0;
    asm volatile("swi 0" : "+r"(r0), "+r"(r1) : : "memory");
    return r0;
}
static inline uintptr_t syscall_2(int sysno, uintptr_t arg0, uintptr_t arg1) {
    register uintptr_t r0 asm("r0") = sysno;
    register uintptr_t r1 asm("r1") = arg0;
    register uintptr_t r2 asm("r2") = arg1;
    asm volatile("swi 0" : "+r"(r0), "+r"(r1), "+r"(r2) : : "memory");
    return r0;
}
static inline uintptr_t syscall_3(int sysno, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2) {
    register uintptr_t r0 asm("r0") = sysno;
    register uintptr_t r1 asm("r1") = arg0;
    register uintptr_t r2 asm("r2") = arg1;
    register uintptr_t r3 asm("r3") = arg2;
    asm volatile("swi 0" : "+r"(r0), "+r"(r1), "+r"(r2), "+r"(r3) : : "memory");
    return r0;
}

static inline void exit() {
    syscall_0(SYSCALL_EXIT);
}
