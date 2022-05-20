#include <stdint.h>

#include "kern.h"
#include "kmalloc.h"
#include "proc.h"
#include "sys.h"
#include "syscall_list.h"
#include "vm.h"

#define VERBOSE_SYSCALLS 0
#if VERBOSE_SYSCALLS
#define dprintf printf
#else
#define dprintf(...)
#endif

unsigned syscall_alloc_page(uint32_t page_addr, uint32_t page_size) {
    assert(page_addr == SYSCALL_ARG_ANY_PAGE);
    dprintf("Got alloc page syscall for any page, 1MB long.\n");
    uintptr_t ptr = (uintptr_t) ka2pa((uintptr_t) kmalloc(page_size));
    dprintf("Giving back physical page %x\n", ptr);
    return ptr;
}

unsigned syscall_vm_map(uint32_t va,
                        uint32_t pa,
                        uint32_t flags,
                        uint32_t page_size) {
    assert(flags == 0);
    dprintf("Got VM_MAP syscall for va=%lx, pa=%lx, flags=%lx, 1MB.\n", va, pa,
            flags);
    // TODO(masot): FLAGS
    vm_map(curproc->pt, va, pa, page_size, AP_RW);
    vm_flushem();
    return 0;
}

void syscall(regs_t *regs) {
    unsigned sysno = regs->r0;
    switch (sysno) {
    case SYSCALL_EXIT:
        printf("Exiting with exit code %lu\n", regs->r1);
        reboot();
        break;
    case SYSCALL_ALLOC_PAGE:
        regs->r0 = syscall_alloc_page(regs->r1, regs->r2);
        break;
    case SYSCALL_DEALLOC_PAGE:
        panic("SYSCALL_DEALLOC_PAGE");
        break;
    case SYSCALL_VM_MAP:
        regs->r0 = syscall_vm_map(regs->r1, regs->r2, regs->r3, regs->r4);
        break;
    case SYSCALL_GET_PID:
        regs->r0 = curproc->id;
        break;
    default:
        panic("unhandled syscall %d\n", sysno);
        break;
    }
}
