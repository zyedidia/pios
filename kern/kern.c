#include "asm.h"
#include "pios.h"

extern int main();

static void map_init_pages() {
    const int page_size = 4096;
    vm_init();
    // map interrupt table
    vm_map(0, 0, 0);
    // map one page of stack
    vm_map(STACK_ADDR - page_size, STACK_ADDR - page_size, 0);
    // map code
    extern char __code_start__, __code_end__;
    for (uintptr_t pa = (uintptr_t) &__code_start__; pa < (uintptr_t) &__code_end__; pa += page_size) {
        vm_map(pa, pa, 0);
    }
    // map heap
    extern char __heap_start__;
    for (uintptr_t pa = (uintptr_t) &__heap_start__; pa < heap_end; pa += page_size) {
        vm_map(pa, pa, 0);
    }
    // map uart and gpio
    for (unsigned i = 0; i < 64; i++) {
        vm_map(0x20200000 + page_size * i, 0x20200000 + page_size * i, 0);
    }

    vm_enable();
}

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif
    sys_enable_cache();

    irq_init_table();
    map_init_pages();

    printf("kernel booted\n");
    return;
}
