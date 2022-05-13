#include <stdint.h>

#include "asm.h"
#include "gpio.h"
#include "interrupts.h"
#include "kern.h"
#include "kmalloc.h"
#include "ksan.h"
#include "proc.h"
#include "sys.h"
#include "uart.h"
#include "vm.h"

void reboot() {
    printf("DONE!!!\n");
    uart_tx_flush();

    volatile uint32_t *PM_RSTC = (uint32_t *) pa2ka(0x2010001c);
    volatile uint32_t *PM_WDOG = (uint32_t *) pa2ka(0x20100024);

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

    extern unsigned char _binary_hello_bin_start;
    extern unsigned char _binary_hello_bin_end;
    proc_t *p_hello = proc_new(&_binary_hello_bin_start, (size_t) (&_binary_hello_bin_end - &_binary_hello_bin_start));
    proc_run(p_hello);

    reboot();
    return;
}
