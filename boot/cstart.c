#include <stdint.h>

#include "kern.h"

void reboot() {
    volatile uint32_t* PM_RSTC = (uint32_t*) 0x2010001c;
    volatile uint32_t* PM_WDOG = (uint32_t*) 0x20100024;

    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    *PM_WDOG = PM_PASSWORD | 1;
    *PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
    while (1)
        ;
}

void boot_start();

void cstart() {
    extern int __bss_start__, __bss_end__;

    int* bss = &__bss_start__;
    int* bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    timer_init();
    uart_init(115200);
    boot_start();
}
