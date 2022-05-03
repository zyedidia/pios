#include "pios.h"

static bool v = true;
void __attribute((interrupt("IRQ"))) irq() {
    if (timer_has_irq()) {
        timer_clear_irq();
        gpio_write(21, v);
        v = !v;
    }
}

int main() {
    gpio_set_output(21);

    timer_irq_load(0x400);
    register_irq_vec(IRQ_VEC_IRQ, (uintptr_t) irq);
    irq_enable(BASIC_TIMER_IRQ);

    enable_interrupts();

    delay_ms(5000);
    return 0;
}
