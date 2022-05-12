#include "vm.h"
#include "kern.h"
#include "sys.h"
#include "kmalloc.h"
#include "bits.h"

pagetable_t* kalloc_pt() {
    pagetable_t* l1pt = (pagetable_t*) kmalloc(sizeof(pagetable_t));
    memset(l1pt, 0, sizeof(pagetable_t));
    return l1pt;
}

static void init_second_level(pde_t* pde) {
    pte_small_t* pgtbl = kmalloc(256 * sizeof(pte_small_t));
    memset(pgtbl, 0, 256 * sizeof(pte_small_t));
    pde->addr = (uintptr_t) pgtbl >> 10;
    pde->tag = 0b01;
}

void vm_map(pagetable_t* pt, uintptr_t va, uintptr_t pa, pg_typ_t typ) {
    unsigned idx = va >> 20;
    l1pte_t* l1pte = &pt->entries[idx];

    switch (typ) {
        case PAGE_UNMAPPED:
            l1pte->pde.tag = 0b00;
            break;
        case PAGE_1MB:
            l1pte->pte_1mb.tag = 0b10;
            l1pte->pte_1mb.sec_base_addr = pa >> 20;
            break;
        case PAGE_4KB:
            if (l1pte->pde.tag == 0b00) {
                init_second_level(&l1pte->pde);
            }
            pte_small_t* l2pt = (pte_small_t*) (l1pte->pde.addr << 10);
            pte_small_t* l2pte = &l2pt[bits_get(va, 12, 19)];
            l2pte->addr = pa >> 12;
            l2pte->ap = AP_NO_ACCESS;
            l2pte->sz = 1;
            break;
        default:
            panic("invalid page type: %d\n", typ);
    }
}

void vm_unmap(pagetable_t* pt, uintptr_t va) {
    vm_map(pt, va, 0, PAGE_UNMAPPED);
}

void vm_set_pt(pagetable_t* pt) {
    sys_set_tlb_base((uintptr_t) pt);
}
