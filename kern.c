#include "pios.h"

extern int main();

void _enable_interrupts();

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif
    cache_enable();

    irq_enable(BASIC_TIMER_IRQ);
    timer_irq_load(0x400);

    _enable_interrupts();

    main();
    return;
}
