#include "pios.h"
#include "dev.h"

static bool v = true;
void __attribute((interrupt("IRQ"))) irq() {
    dev_barrier();

    if (!timer_has_irq()) {
        return;
    }

    gpio_write(21, v);
    v = !v;

    dev_barrier();

    timer_clear_irq();
    dev_barrier();
}

int main() {
    gpio_set_output(21);

    irq_init();
    irq_enable(BASIC_TIMER_IRQ);
    timer_irq_load(0x400);
    register_irq_vec(IRQ_VEC_IRQ, (uintptr_t) irq);

    enable_interrupts();

    delay_ms(3000);
    return 0;
}
