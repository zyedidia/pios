#include "uart.h"

// extern uint32_t syscall(uint32_t syscall, uint32_t arg0, uint32_t arg1, uint32_t arg2);

unsigned main() {
    // unsigned gpio_page = 0x20200000;
    // syscall(SYSCALL_MMU_MAP, gpio_page, gpio_page, ARG_PAGE_4KB);
    // unsigned uart_page = 0x20215004 & (~(4096 - 1));
    // syscall(SYSCALL_MMU_MAP, uart_page, uart_page, ARG_PAGE_4KB);

    init_printf(NULL, uart_putc);
    printf("Hello, world!\n");

    unsigned cpsr = 0;
    asm volatile("mrs %0, cpsr" : "=r"(cpsr));
    printf("CPSR appears to be %x\n", cpsr);

    unsigned sp = 0;
    asm volatile("mov %0, sp" : "=r"(sp));
    printf("My stack appears to be at %x\n", sp);

    volatile unsigned *ker_val = (void*)0x80000005;
    printf("Trying to access a kernel byte at 0x%x...\n", (unsigned)ker_val);
    printf("Value: %x\n", *ker_val);

    return 24;
}
