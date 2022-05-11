#include <stdint.h>

#include "asm.h"
#include "kern.h"
#include "uart.h"
#include "sys.h"
#include "vm.h"
#include "gpio.h"
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

void kernel_start() {
    sys_enable_cache();

    irq_init_table();
    uart_init(115200);
    init_printf(NULL, uart_putc);
    printf("kernel booted\n");

    reboot();
    return;
}
