#include "../kern/syscall_list.h"

#define STACK_ADDR                  0x8000000

.section ".boot"

.globl _start
_start:
    // mov r0, #SYSCALL_ALLOC_PAGE
    // mov r1, #SYSCALL_ARG_ANY_PAGE
    // mov r2, #SYSCALL_ARG_PAGE_4KB
    // swi 0                           // result in r0

    // mov r1, #STACK_ADDR             // virtual page
    // sub r1, r1, #4096               // start of page for stack
    // mov r2, r0                      // physical page
    // mov r3, #0                      // flags
    // mov r4, #SYSCALL_ARG_PAGE_4KB   // size
    // mov r0, #SYSCALL_VM_MAP
    // swi 0

    // mov sp, #STACK_ADDR             // Now we have a stack !
    // sub sp, sp, #4

    push {lr}
    bl main
    pop {lr}
    bx lr

    // mov r0, #SYSCALL_EXIT
    // swi 0

.globl syscall
syscall:
    swi 0
    bx lr
