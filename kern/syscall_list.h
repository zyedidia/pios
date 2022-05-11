#pragma once

#define SYSCALL_EXIT            0
#define SYSCALL_ALLOC_PAGE      1
#define SYSCALL_DEALLOC_PAGE    2
#define SYSCALL_VM_MAP          3

#define SYSCALL_ARG_ANY_PAGE    -1
#define SYSCALL_ARG_PAGE_4KB    0
