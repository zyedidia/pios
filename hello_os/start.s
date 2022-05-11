// TODO MASOT #include "syscalls.h"

#define SYSCALL_ALLOC_PHYS_PAGE     1
#define SYSCALL_MMU_MAP             2
#define SYSCALL_EXIT                3
#define ARG_ANY_PAGE                -1
#define ARG_PAGE_4KB                0
#define STACK_ADDR                  0x8000000

.section ".boot"

.globl _start
_start:
    mov r0, #SYSCALL_ALLOC_PHYS_PAGE
    mov r1, #ARG_ANY_PAGE
    mov r2, #ARG_PAGE_4KB
    swi 0                           // result in r0

    mov r1, r0                      // physical page
    mov r2, #STACK_ADDR             // virtual page
    sub r2, r2, #4096               // start of page for stack
    mov r3, #ARG_PAGE_4KB           // size
    mov r0, #SYSCALL_MMU_MAP
    swi 0

    mov sp, #STACK_ADDR             // Now we have a stack !
    sub sp, sp, #4

    bl main

    mov r0, #SYSCALL_EXIT
    swi 0

.globl syscall
syscall:
    swi 0
    bx lr
