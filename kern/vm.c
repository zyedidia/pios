#include "vm.h"
#include "sys.h"

// TODO: see comment in boot.c
extern pte_1mb_t __attribute__((aligned(1 << 14))) early_pagetable[4096];
void vm_map_section_into_early_pt(uintptr_t va, uintptr_t pa) {
    unsigned pte_idx = va >> 20;
    pte_1mb_t* pte = &early_pagetable[pte_idx];

    if (pte->tag != 0b00) {
        // printf("BAD!\n");
        reboot();
    }

    pte->tag = 0b10;
    pte->sec_base_addr = pa >> 20;
}

void vm_flushem() {
    sys_clean_and_invalidate_cache();
    sys_flush_btb();
    sys_invalidate_tlb();
    sys_prefetch_flush();
    dsb();
}

// static pde_t* pgdir;
// 
// void vm_init() {
//     pgdir = kmalloc_aligned(4096 * sizeof(pde_t), 1 << 14);
//     memset(pgdir, 0, 4096 * sizeof(pde_t));
// }
// 
// static void init_second_level(pde_t* pde) {
//     pte_small_t* pgtbl = kmalloc_aligned(256 * sizeof(pte_small_t), 1 << 10);
//     memset(pgtbl, 0, 256 * sizeof(pte_small_t));
//     pde->addr = (uintptr_t) pgtbl >> 10;
//     pde->tag = 0b01;
// }
// 
// void vm_map(uintptr_t va, uintptr_t pa, unsigned flags) {
//     unsigned pde_idx = va >> 20;
//     pde_t* pde = &pgdir[pde_idx];
// 
//     switch (pde->tag) {
//         case 0b00:
//             init_second_level(pde);
//         case 0b01:;
//             pte_small_t* pgtbl = (pte_small_t*) (pde->addr << 10);
//             pte_small_t* pte = &pgtbl[bits_get(va, 12, 19)];
//             pte->addr = pa >> 12;
//             pte->ap = AP_RW;
//             pte->sz = 1;
//             break;
//         default:
//             panic("invalid pde tag: %d\n", pde->tag);
//     }
// }
// 
// void vm_unmap(uintptr_t va) {
//     unsigned pde_idx = va >> 20;
//     pde_t* pde = &pgdir[pde_idx];
// 
//     switch (pde->tag) {
//         case 0b00:
//             return;
//         case 0b01:;
//             pte_small_t* pgtbl = (pte_small_t*) (pde->addr << 10);
//             pte_small_t* pte = &pgtbl[bits_get(va, 12, 19)];
//             memset(pte, 0, sizeof(pte_small_t));
//             break;
//         default:
//             return;
//     }
// }
// 
// // void vm_enable() {
// //     sys_invalidate_cache();
// //     sys_invalidate_tlb();
// //     dsb();
// //     sys_set_domain(DOM_CLIENT);
// //     sys_set_tlb_base((uintptr_t) pgdir);
// //     sys_set_cache_control(SYS_MMU_ENABLE | SYS_DCACHE_ENABLE |
// //                           SYS_ICACHE_ENABLE | SYS_BRANCH_PREDICTION_ENABLE |
// //                           SYS_WRITE_BUFFER_ENABLE | SYS_MMU_XP);
// // }
