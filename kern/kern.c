#include <stdint.h>

#include "asm.h"
#include "gpio.h"
#include "interrupts.h"
#include "kern.h"
#include "ksan.h"
#include "sys.h"
#include "uart.h"
#include "vm.h"
#include "simple_malloc.h"

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
#if (SANITIZE == 1)
    asan_enable();
#endif

    sys_enable_cache();

    uart_init(115200);
    init_printf(NULL, uart_putc);
    irq_init();

    printf("kernel booted\n");

    // get one page for the prog, TODO(masot): check that it fits.
    uint8_t *pdata = smalloc_aligned(1024 * 1024, 1024 * 1024);
    vm_map_section_into_early_pt(0, (uintptr_t)pdata);
    vm_flushem();

    extern char prog[];
    extern unsigned n_prog;
    uint8_t *vdata = (void*)0x8000;
    for (size_t i = 0; i < n_prog; i++) vdata[i] = prog[i];

    unsigned (*entry)(void) = (void*)vdata;
    entry();

    printf("ERROR: Libos ret'd without calling SYSCALL_EXIT!\n");
    reboot();
    return;
}
