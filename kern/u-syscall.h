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
static inline uintptr_t syscall_3(int sysno,
                                  uintptr_t arg0,
                                  uintptr_t arg1,
                                  uintptr_t arg2) {
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

// Allocates a 4kB physical page at index i in [0, MAX_PAGES). If i is -1, tries
// to allocate any physical page. Returns the address of the allocated page; or
// returns a null pointer if the requested page does not exist or is already
// allocated, or if there are no available pages.
static inline void *alloc_page(int i) {
    // Mark which process allocated the page.
    return syscall_1(SYSCALL_ALLOC_PAGE, i);
}

// Release the allocated physical page. The caller needs to have been previously
// allocated the physical page and not already deallocated it. Returns 0 if
// successful, otherwise -1.
static inline int dealloc_page(void *addr) {
    return syscall_1(SYSCALL_DEALLOC_PAGE, addr);
}

// Map the virtual address to the physical address. The caller must have been
// allocated the physical address. Returns 0 if successful, otherwise -1.
static inline int vm_map(uintptr_t va, uintptr_t pa, unsigned flags) {
    return syscall_3(SYSCALL_VM_MAP, va, pa, flags);
}
