#include "pios.h"
#include "dev.h"

void __attribute__((interrupt("IRQ"))) irq() {
    dev_barrier();

    if (!irq_pending(GPIO_INT3)) {
        return;
    }
    printf("irq %d\n", gpio_event_detect(21));
    gpio_event_clear(21);

    dev_barrier();
}

int main() {
    gpio_set_input(21);
    gpio_set_output(20);

    irq_init_table();
    gpio_int_rise(21);
    gpio_int_fall(21);
    register_irq_vec(IRQ_VEC_IRQ, (uintptr_t) irq);
    enable_interrupts();

    gpio_write(20, 1);
    delay_ms(1000);
    gpio_write(20, 0);
    delay_ms(1000);
    return 0;
}
