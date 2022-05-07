#include "pios.h"
#include "dev.h"

volatile unsigned count = 0;

void irq(uint32_t *state_ptr) {
    dev_barrier();
    if (!timer_has_irq()) return;

    if (count++ > 3) reboot();

    for (size_t i = 0; i < 16; i++) {
        printf("Register %u is %lu\n", i, state_ptr[i]);
    }

    dev_barrier();
    timer_clear_irq();
    dev_barrier();
}

extern void irq_vec_asm();

int main() {
    gpio_set_output(21);

    irq_init_table();
    irq_enable(BASIC_TIMER_IRQ);
    timer_irq_load(0x400);
    register_irq_vec(IRQ_VEC_IRQ, (uintptr_t) irq_vec_asm);

    enable_interrupts();

    while (true) {
        asm ("mov r0,  $0");
        asm ("mov r1,  $1");
        asm ("mov r2,  $2");
        asm ("mov r3,  $3");
        asm ("mov r4,  $4");
        asm ("mov r5,  $5");
        asm ("mov r6,  $6");
        asm ("mov r7,  $7");
        asm ("mov r8,  $8");
        asm ("mov r9,  $9");
        asm ("mov r10, $10");
        asm ("mov r11, $11");
        asm ("mov r12, $12");
        asm ("mov r13, $13");
        asm ("mov r14, $14");
    }
    delay_ms(3000);
    return 0;
}
