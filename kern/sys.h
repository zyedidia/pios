#pragma once

#include <stdint.h>

#define CPU_FREQ_MHZ 700

#define SYS_MMU_ENABLE               (1 << 0)
#define SYS_DCACHE_ENABLE            (1 << 2)
#define SYS_WRITE_BUFFER_ENABLE      (1 << 3)
#define SYS_BRANCH_PREDICTION_ENABLE (1 << 11)
#define SYS_ICACHE_ENABLE            (1 << 12)
#define SYS_ROUND_ROBIN_ENABLE       (1 << 14)
#define SYS_MMU_XP                   (1 << 23)

static inline void dsb() {
    asm volatile("mcr p15, 0, r0, c7, c10, 4");
}

static inline void dmb() {
    asm volatile("mcr p15, 0, r0, c7, c10, 5");
}

static inline void sys_set_vec_base(uintptr_t addr) {
    asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r"(addr));
}

static inline uintptr_t sys_get_vec_base() {
    uintptr_t addr;
    asm volatile("mrc p15, 0, %0, c12, c0, 0" : "=r"(addr));
    return addr;
}

static inline void sys_prefetch_flush() {
    asm volatile("mcr p15, 0, r0, c7, c5, 4");
}

static inline void sys_flush_btb() {
    asm volatile("mcr p15, 0, r0, c7, c5, 6");
}

static inline void sys_set_cache_control(unsigned reg) {
    // See section 3.2.7 in arm1176
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline unsigned sys_get_cache_control() {
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    return reg;
}

static inline void sys_enable_dcache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_DCACHE_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_dcache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYS_DCACHE_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_enable_icache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_ICACHE_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_icache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYS_ICACHE_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_enable_write_buffer() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_WRITE_BUFFER_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_write_buffer() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYS_WRITE_BUFFER_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_enable_branch_prediction() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_BRANCH_PREDICTION_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_branch_prediction() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYS_BRANCH_PREDICTION_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_enable_cache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_DCACHE_ENABLE | SYS_ICACHE_ENABLE | SYS_WRITE_BUFFER_ENABLE |
           SYS_BRANCH_PREDICTION_ENABLE;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_cache() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~(SYS_DCACHE_ENABLE | SYS_ICACHE_ENABLE | SYS_WRITE_BUFFER_ENABLE |
             SYS_BRANCH_PREDICTION_ENABLE);
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_enable_mmu() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg |= SYS_MMU_ENABLE;
    reg |= SYS_MMU_XP;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_disable_mmu() {
    // See section 3.2.7 in arm1176
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg));
    reg &= ~SYS_MMU_ENABLE;
    reg &= ~SYS_MMU_XP;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
}

static inline void sys_invalidate_icache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c5, 0" : : "r"(0));
}

static inline void sys_invalidate_dcache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c6, 0" : : "r"(0));
}

static inline void sys_invalidate_cache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c7, 0" : : "r"(0));
}

static inline void sys_clean_dcache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c10, 0" : : "r"(0));
}

static inline void sys_clean_cache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c11, 0" : : "r"(0));
}

static inline void sys_clean_and_invalidate_dcache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c14, 0" : : "r"(0));
}

static inline void sys_clean_and_invalidate_cache() {
    // See Fig 3.38 in section 3.2.22 in arm1176
    sys_clean_and_invalidate_dcache();
    sys_invalidate_icache();
}

static inline void sys_invalidate_tlb() {
    asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r"(0));
    dsb();
}

static inline void sys_set_domain(unsigned reg) {
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r"(reg));
}

static inline unsigned sys_get_domain() {
    unsigned reg;
    asm volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(reg));
    return reg;
}

static inline void sys_set_tlb_base(unsigned base) {
    asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r"(base));
    asm volatile("mcr p15, 0, %0, c2, c0, 1" : : "r"(base));
}

static inline unsigned get_dfar() {
    unsigned dfar = 0;
    asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r"(dfar));
    return dfar;
}

static inline unsigned get_ifar() {
    unsigned ifar = 0;
    asm volatile("mrc p15, 0, %0, c6, c0, 2" : "=r"(ifar));
    return ifar;
}

static inline unsigned get_cpsr() {
    unsigned cpsr = 0;
    asm volatile("mrs %0, cpsr" : "=r"(cpsr));
    return cpsr;
}

static inline void set_cpsr(unsigned cpsr) {
    asm volatile("msr cpsr, %0" : : "r"(cpsr));
}

static inline unsigned get_spsr() {
    unsigned spsr = 0;
    asm volatile("mrs %0, spsr" : "=r"(spsr));
    return spsr;
}

static inline void set_spsr(unsigned spsr) {
    asm volatile("msr spsr, %0" : : "r"(spsr));
}

static inline void go_to_mode(unsigned mode) {
    set_cpsr((get_cpsr() & ~0b11111) | mode);
}
