#pragma once

#define SYSCALL_EXIT         0
#define SYSCALL_ALLOC_PAGE   1
#define SYSCALL_DEALLOC_PAGE 2
#define SYSCALL_VM_MAP       3
#define SYSCALL_GET_PID      4

// NOTE: DO NOT CHANGE THESE without changing vm.h
#define SYSCALL_ARG_ANY_PAGE 0
#define SIZE_4KB             (1 << 12)
#define SIZE_16KB            (1 << 14)
#define SIZE_1MB             (1 << 20)
#define SIZE_16MB            (1 << 24)
