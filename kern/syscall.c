#include "pios.h"
#include "syscall.h"
#include "vm.h"

unsigned syscall_alloc_page(uint32_t page_addr, uint32_t page_size) {
    assert(page_addr == SYSCALL_ARG_ANY_PAGE);
    assert(page_size == SYSCALL_ARG_PAGE_4KB);

    panic("SYSCALL_ALLOC_PAGE");

    return -1;
}

unsigned syscall_vm_map(uint32_t va, uint32_t pa, uint32_t flags, uint32_t page_size) {
    assert(page_size == SYSCALL_ARG_PAGE_4KB);

    panic("SYSCALL_VM_MAP");

    return -1;
}

void syscall(user_regs_t* regs) {
    unsigned sysno = regs->r0;
    switch (sysno) {
    case SYSCALL_EXIT:
        printf("Exiting\n");
        reboot();
        break;
    case SYSCALL_ALLOC_PAGE:;
        regs->r0 = syscall_alloc_page(regs->r1, regs->r2);
        break;

    case SYSCALL_DEALLOC_PAGE:;
        void* addr = (void*)regs->r1;
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
