#include "pios.h"
#include "syscall.h"
#include "vm.h"

void syscall(user_regs_t* regs) {
    unsigned sysno = regs->r0;
    switch (sysno) {
    case SYSCALL_EXIT:
        printf("Exiting\n");
        reboot();
        break;
    case SYSCALL_ALLOC_PAGE:
        uint32_t i = regs->r1;
        unimplemented("SYSCALL_ALLOC_PAGE");
        break;
    case SYSCALL_DEALLOC_PAGE:
        void* addr = (void*)regs->r1;
        unimplemented("SYSCALL_DEALLOC_PAGE");
        break;
    case SYSCALL_VM_MAP:
        uintptr_t va = regs->r1;
        uintptr_t pa = regs->r2;
        unsigned flags = regs->r3;
        unimplemented("SYSCALL_VM_MAP");
        break;
    default:
        panic("unhandled syscall %d\n", sysno);
        break;
    }
}
