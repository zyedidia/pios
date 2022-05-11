#include "uart.h"

extern uint32_t syscall(uint32_t syscall, uint32_t arg0, uint32_t arg1, uint32_t arg2);

#define SYSCALL_MMU_MAP 2
#define ARG_PAGE_4KB    0

int main() {
    // unsigned gpio_page = 0x20200000;
    // syscall(SYSCALL_MMU_MAP, gpio_page, gpio_page, ARG_PAGE_4KB);
    // unsigned uart_page = 0x20215004 & (~(4096 - 1));
    // syscall(SYSCALL_MMU_MAP, uart_page, uart_page, ARG_PAGE_4KB);
    printf("Hello, world!\n");
    return 0;
}
