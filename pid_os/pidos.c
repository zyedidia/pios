#include "uart.h"
#include "../kern/syscall_list.h"

extern uint32_t syscall(uint32_t syscall, ...);

unsigned main() {
    init_printf(NULL, uart_putc);

    uint32_t pid = syscall(SYSCALL_GET_PID);
    while (true) {
        printf("Hello from process %lu\n", pid);
        for (size_t i = 0; i < (1 << 21); i++) {
            asm volatile ("nop");
        }
    }

    return 24;
}
