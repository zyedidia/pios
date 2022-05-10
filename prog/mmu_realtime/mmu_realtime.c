#include "pios.h"

#define STACK_ADDR     0x8000000
#define INT_STACK_ADDR 0x9000000
#define CODE_ADDR      0x8000

#define dsb() asm volatile("mcr p15, #0, %0, c7, c10, #4" : : "r"(0))
static void system_invalidate_tlb(void) {
    asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r"(0));
    dsb();
}

static void system_invalidate_cache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c7, 0" : : "r"(0));
}

void irq(uint32_t *regs) {
    regs[15] -= 4; // pc points to the _next_ instruction in irq

    unsigned instr = *(unsigned*)(regs[15]);
    uint32_t dst_reg = instr >> 12 & 0xf,
             src_reg = instr >> 16 & 0xf;
    // TODO: Actually parse these correctly
    int32_t imm_val = instr & 0xfff;
    if ((instr & 0xfff00000) == 0xe5100000) { imm_val *= -1; }
    else if ((instr & 0xfff00000) == 0xe5000000) { imm_val *= -1; }

    unsigned addr = regs[src_reg] + imm_val;
    unsigned page = addr & (~(4096 - 1));
    printf("Attempted to access address: 0x%x, page: 0x%x, adding to the page table...\n", addr, page);
    vm_map(page, page, 0);
    system_invalidate_tlb();
    system_invalidate_cache();
}

extern void irq_vec_asm();
int main() {
    irq_init_table();
    register_irq_vec(IRQ_VEC_DATA_ABORT, (uintptr_t)irq_vec_asm);
    register_irq_vec(IRQ_VEC_PREFETCH_ABORT, (uintptr_t)irq_vec_asm);
    enable_interrupts();

    const int page_size = 4096;

    vm_init();

    vm_map(0, 0, 0);

    for (unsigned i = 0; i < 4; i++) {
        vm_map(STACK_ADDR - page_size * (i + 1),
               STACK_ADDR - page_size * (i + 1), 0);
        vm_map(INT_STACK_ADDR - page_size * (i + 1),
               INT_STACK_ADDR - page_size * (i + 1), 0);
        vm_map(CODE_ADDR + page_size * i, CODE_ADDR + page_size * i, 0);
    }

    extern char __heap_start__;
    for (unsigned i = 0; i < 512; i++) {
        vm_map((uintptr_t) &__heap_start__ + page_size * i,
               (uintptr_t) &__heap_start__ + page_size * i, 0);
        vm_map(0x20000000 + page_size * i, 0x20000000 + page_size * i, 0);
        vm_map(0x20200000 + page_size * i, 0x20200000 + page_size * i, 0);
    }

    vm_enable();

    printf("done!\n");

    volatile unsigned *ptr = (void*)0x30200005;
    *ptr = 5;
    printf("Wrote data: %u\n", *ptr);
    *ptr = 10;
    printf("Overwrote data: %u\n", *ptr);

    return 0;
}
