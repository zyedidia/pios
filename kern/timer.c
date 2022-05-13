#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t load;
    uint32_t value;
    uint32_t ctrl;
    uint32_t irq_clear;
    uint32_t raw_irq;
    uint32_t masked_irq;
    uint32_t reload;
    uint32_t pre_divider;
    uint32_t free_running_counter;
} timer_t;

#define TIMER_CTRL_32BIT (1 << 1)

#define TIMER_CTRL_PRESCALE_1   (0 << 2)
#define TIMER_CTRL_PRESCALE_16  (1 << 2)
#define TIMER_CTRL_PRESCALE_256 (2 << 2)

#define TIMER_CTRL_INT_ENABLE  (1 << 5)
#define TIMER_CTRL_INT_DISABLE (0 << 5)

#define TIMER_CTRL_ENABLE  (1 << 7)
#define TIMER_CTRL_DISABLE (0 << 7)

#define pa2ka(pa) (((pa) | (1UL << 31)))
static volatile timer_t* const timer = (timer_t*) pa2ka(0x2000b400);

void timer_irq_load(uint32_t load) {
    timer->load = load;
    timer->ctrl = TIMER_CTRL_32BIT | TIMER_CTRL_ENABLE | TIMER_CTRL_INT_ENABLE |
                  TIMER_CTRL_PRESCALE_256;
}

bool timer_has_irq() {
    return timer->masked_irq;
}

void timer_clear_irq() {
    timer->irq_clear = 1;
}
