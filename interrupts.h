#pragma once

#include <stdint.h>

#define BASIC_TIMER_IRQ      (1 << 0)
#define BASIC_MAILBOX_IRQ    (1 << 1)
#define BASIC_DOORBELL_0_IRQ (1 << 2)
#define BASIC_DOORBELL_1_IRQ (1 << 3)
#define BASIC_GPU_0_HALTED_IRQ   (1 << 4)
#define BASIC_GPU_1_HALTED_IRQ   (1 << 5)
#define BASIC_ACCESS_ERROR_1_IRQ (1 << 6)
#define BASIC_ACCESS_ERROR_0_IRQ (1 << 7)

void irq_enable(uint32_t val);
