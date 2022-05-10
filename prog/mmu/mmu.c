#include "pios.h"

#define STACK_ADDR 0x8000000
#define INT_STACK_ADDR 0x9000000
#define CODE_ADDR 0x8000

int main() {
    irq_init_table();
    enable_interrupts();

    const int page_size = 4096;

    vm_init();

    // for (unsigned ra = 0;; ra += 4096) {
    //     vm_map(ra, ra, 0);
    //     if (ra == 0x10000000) break;
    // }

    // vm_map(0, 0, 0);

    // for (unsigned i = 0; i < 4; i++) {
    //     vm_map(STACK_ADDR - page_size * (i + 1), STACK_ADDR - page_size * (i + 1), 0);
    //     vm_map(INT_STACK_ADDR - page_size * (i + 1), INT_STACK_ADDR - page_size * (i + 1), 0);
    //     vm_map(CODE_ADDR + page_size * i, CODE_ADDR + page_size * i, 0);
    // }

    // extern char __heap_start__;
    // for (unsigned i = 0; i < 256; i++) {
    //     vm_map((uintptr_t) &__heap_start__ + page_size * i, (uintptr_t) &__heap_start__ + page_size * i, 0);
    //     // vm_map(0x20000000 + page_size * i, 0x20000000 + page_size * i, 0);
    //     // vm_map(0x20200000 + page_size * i, 0x20200000 + page_size * i, 0);
    // }

    vm_enable();

    printf("done! %u\n", *(unsigned*)(0x00200008));
    printf("done! %u\n", *(unsigned*)(0x00300108));

    return 0;
}
