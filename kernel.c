#include "libc/tinyprintf.h"
#include "uart.h"

static void device_init() {
    uart_init(115200);
    init_printf(NULL, uart_putc);
}

void kernel_start() {
    device_init();

    printf("kernel booted!\n");

    return;
}
