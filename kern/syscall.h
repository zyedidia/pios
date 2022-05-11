#pragma once

#include <stdint.h>

enum {
    SYSCALL_EXIT = 0,
    SYSCALL_ALLOC_PAGE = 1,
    SYSCALL_DEALLOC_PAGE = 2,
    SYSCALL_VM_MAP = 3,
};

enum {
    SYSCALL_ARG_ANY_PAGE = -1,
    SYSCALL_ARG_PAGE_4KB = 0,
};
