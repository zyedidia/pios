#include <stdint.h>

#include "kern.h"
#include "syscall_list.h"
#include "vm.h"

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

unsigned syscall_alloc_page(uint32_t page_addr, uint32_t page_size) {
    assert(page_addr == SYSCALL_ARG_ANY_PAGE);
    assert(page_size == SYSCALL_ARG_PAGE_4KB);

    panic("SYSCALL_ALLOC_PAGE");

    return -1;
}

unsigned syscall_vm_map(uint32_t va,
                        uint32_t pa,
                        uint32_t flags,
                        uint32_t page_size) {
    assert(page_size == SYSCALL_ARG_PAGE_4KB);

    panic("SYSCALL_VM_MAP");

    return -1;
}

void syscall(user_regs_t* regs) {
    unsigned sysno = regs->r0;
    switch (sysno) {
        case SYSCALL_EXIT:
            printf("Exiting with exit code %u\n", regs->r1);
            reboot();
            break;
        case SYSCALL_ALLOC_PAGE:;
            regs->r0 = syscall_alloc_page(regs->r1, regs->r2);
            break;

        case SYSCALL_DEALLOC_PAGE:;
            /* void* addr = (void*)regs->r1; */
            panic("SYSCALL_DEALLOC_PAGE");
            break;

        case SYSCALL_VM_MAP:
            regs->r0 = syscall_vm_map(regs->r1, regs->r2, regs->r3, regs->r4);
            break;

        default:
            panic("unhandled syscall %d\n", sysno);
            break;
    }
}
