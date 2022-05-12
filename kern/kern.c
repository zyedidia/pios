#include <stdint.h>

#include "asm.h"
#include "kern.h"
#include "uart.h"
#include "sys.h"
#include "vm.h"
#include "gpio.h"
#include "syscall.h"
#include "interrupts.h"

void reboot() {
    printf("DONE!!!\n");
    uart_tx_flush();

    volatile uint32_t* PM_RSTC = (uint32_t*) pa2ka(0x2010001cU);
    volatile uint32_t* PM_WDOG = (uint32_t*) pa2ka(0x20100024U);

    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    *PM_WDOG = PM_PASSWORD | 1;
    *PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
    while (1)
        ;
}

extern pte_1mb_t __attribute__((aligned(1 << 14))) early_pagetable[4096];
void vm_map_section(uintptr_t va, uintptr_t pa) {
    unsigned pte_idx = va >> 20;
    pte_1mb_t* pte = &early_pagetable[pte_idx];

    if (pte->tag != 0b00) {
        printf("BAD!\n");
        reboot();
    }

    pte->tag = 0b10;
    pte->sec_base_addr = pa >> 20;
}

extern char prog[];
extern unsigned n_prog;

extern char _kheap_start;
uintptr_t heap_end = (uintptr_t) &_kheap_start; // VIRTUAL address

static inline uintptr_t align_off(uintptr_t ptr, size_t align) {
    return ((~ptr) + 1) & (align - 1);
}

void syscall(user_regs_t* regs) {
    regs->r15 += 4;
    switch (regs->r0) {
        case SYSCALL_EXIT:
            printf("Got exit syscall with argument %lu\n", regs->r1);
            reboot(); // TODO: Just exit that process, not the whole thing ...
            break;
        case SYSCALL_ALLOC_PAGE:
            assert(regs->r1 == SYSCALL_ARG_ANY_PAGE);
            assert(regs->r2 == SYSCALL_ARG_PAGE_1MB);
            printf("Got alloc page syscall for any page, 1MB long.\n");
            heap_end += align_off(heap_end, 1024*1024);
            regs->r0 = ka2pa(heap_end);
            printf("Giving back physical page %x\n", regs->r0);
            heap_end += 1024*1024;
            break;
        case SYSCALL_VM_MAP:;
            uint32_t va = regs->r1,
                     pa = regs->r2,
                     flags = regs->r3,
                     size = regs->r4;
            assert(size == SYSCALL_ARG_PAGE_1MB);
            printf("Got VM_MAP syscall for va=%x, pa=%x, flags=%x, 1MB.\n",
                   va, pa, flags);
            vm_map_section(va, pa);
            break;
    }
}

void kernel_start() {
    sys_enable_cache();

    uart_init(115200);
    init_printf(NULL, uart_putc);

    irq_init_table();
    register_irq_vec(IRQ_VEC_SOFTWARE_IRQ, (void*)syscall);
    enable_interrupts();

    // pick physical space for prog. eventually we should use an allocator, for
    // now let's just put it right at the end of the heap.
    // I guess we don't really need to align this, but why not
    uint8_t *data = ka2pa(heap_end) + align_off(ka2pa(heap_end), 0x1000000);
    vm_map_section(0, (uintptr_t)data);
    sys_clean_and_invalidate_cache();
    sys_flush_btb();
    sys_invalidate_tlb();
    sys_prefetch_flush();
    dsb();
    uint8_t *vdata = (void*)0x8000;
    for (size_t i = 0; i < n_prog; i++) vdata[i] = prog[i];
    heap_end += n_prog;

    unsigned (*entry)(void) = (void*)0x8000;
    unsigned res = entry();

    printf("Got exit %u\n", res);

    printf("kernel booted\n");

    reboot();
    return;
}
