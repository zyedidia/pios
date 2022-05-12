#include <stdint.h>

#include "asm.h"
#include "gpio.h"
#include "interrupts.h"
#include "kern.h"
#include "ksan.h"
#include "sys.h"
#include "uart.h"
#include "vm.h"
#include "proc.h"
#include "kmalloc.h"

void reboot() {
    printf("DONE!!!\n");
    uart_tx_flush();

    volatile uint32_t* PM_RSTC = (uint32_t*) pa2ka(0x2010001c);
    volatile uint32_t* PM_WDOG = (uint32_t*) pa2ka(0x20100024);

    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    *PM_WDOG = PM_PASSWORD | 1;
    *PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
    while (1)
        ;
}

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif

    sys_enable_cache();

    uart_init(115200);
    init_printf(NULL, uart_putc);
    irq_init();
    kmalloc_init();

    printf("kernel booted\n");

    proc_t p_basic;
    extern unsigned char prog_basic[];
    extern size_t prog_basic_sz;
    proc_new(&p_basic, &prog_basic[0], prog_basic_sz);
    proc_run(&p_basic);

    reboot();
    return;
}
