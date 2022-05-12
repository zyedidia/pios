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
    uint32_t r15;
} user_regs_t;

unsigned syscall_alloc_page(uint32_t page_addr, uint32_t page_size) {
    assert(page_addr == SYSCALL_ARG_ANY_PAGE);
    assert(page_size == SYSCALL_ARG_PAGE_1MB);
    printf("Got alloc page syscall for any page, 1MB long.\n");
    uintptr_t ptr = smalloc_aligned(1024 * 1024, 1024 * 1024);
    printf("Giving back physical page %x\n", ptr);
    return ptr;
}

unsigned syscall_vm_map(uint32_t va,
                        uint32_t pa,
                        uint32_t flags,
                        uint32_t page_size) {
    assert(page_size == SYSCALL_ARG_PAGE_1MB);
    printf("Got VM_MAP syscall for va=%lx, pa=%lx, flags=%lx, 1MB.\n",
           va, pa, flags);
    vm_map_section_into_early_pt(va, pa);
    vm_flushem();
    return 0;
}

void syscall(user_regs_t* regs) {
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

        default:
            panic("unhandled syscall %d\n", sysno);
            break;
    }
    regs->r15 += 4;
}
