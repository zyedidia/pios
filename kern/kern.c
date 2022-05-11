#include "asm.h"
#include "pios.h"

static void map_phys_page(uintptr_t pa) {
    vm_map(pa, pa, 0);
    vm_map(pa2ka(pa), pa, 0);
}

static void map_kernel_pages() {
    const int page_size = 4096;
    vm_init();
    // map interrupt table
    map_phys_page(0);
    // map one page of stack
    map_phys_page(STACK_ADDR - page_size);
    // map code
    extern char __code_start__, __code_end__;
    for (uintptr_t pa = (uintptr_t) &__code_start__; pa < (uintptr_t) &__code_end__; pa += page_size) {
        map_phys_page(pa);
    }
    // map heap
    extern char __heap_start__;
    for (uintptr_t pa = (uintptr_t) &__heap_start__; pa < heap_end; pa += page_size) {
        map_phys_page(pa);
    }
    // map uart, gpio, watchdog timer
    map_phys_page(0x20100000);
    for (unsigned i = 0; i < 64; i++) {
        map_phys_page(0x20200000 + page_size * i);
    }
    sys_clean_and_invalidate_cache();
}

static void unmap_low_pages() {
    const int page_size = 4096;
    // map interrupt table
    vm_unmap(0);
    // map one page of stack
    vm_unmap(STACK_ADDR - page_size);
    // map code
    extern char __code_start__, __code_end__;
    for (uintptr_t pa = (uintptr_t) &__code_start__; pa < (uintptr_t) &__code_end__; pa += page_size) {
        vm_unmap(pa);
    }
    // map heap
    extern char __heap_start__;
    for (uintptr_t pa = (uintptr_t) &__heap_start__; pa < heap_end; pa += page_size) {
        vm_unmap(pa);
    }
    // map uart, gpio, watchdog timer
    vm_unmap(0x20100000);
    for (unsigned i = 0; i < 64; i++) {
        vm_unmap(0x20200000 + page_size * i);
    }
    sys_prefetch_flush();
    sys_flush_btb();
    sys_clean_and_invalidate_cache();
    dsb();
}

void jump_to_ka();

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif
    sys_enable_cache();

    irq_init_table();
    map_kernel_pages();
    vm_enable();

    jump_to_ka();
    /* unmap_low_pages(); */

    printf("kernel booted\n");
    return;
}
