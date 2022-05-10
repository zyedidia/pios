#include "pios.h"

#define STACK_ADDR 0x8000000
#define INT_STACK_ADDR 0x9000000
#define CODE_ADDR 0x8000

int main() {
    irq_init_table();
    enable_interrupts();

    const int page_size = 4096;

    vm_init();

    for (unsigned ra = 0;; ra += 4096) {
        vm_map(ra, ra, 0);
        if (ra == 0x10000000) break;
    }

    for (unsigned ra = 0x20000000; ; ra += 4096) {
        vm_map(ra, ra, 0);
        if (ra == 0x30000000) break;
    }

    vm_enable();

    printf("done! %u\n", *(unsigned*)(0x00200008));
    printf("done! %u\n", *(unsigned*)(0x00300108));

    return 0;
}
