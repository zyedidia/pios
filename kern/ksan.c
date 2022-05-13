#if (SANITIZE == 1)

#include <stdbool.h>
#include <stdint.h>

#include "vm.h"
#include "kern.h"
#include "kmalloc.h"

typedef struct {
    const char* file;
    uint32_t line;
    uint32_t col;
} src_loc_t;

typedef struct {
    uint16_t kind;
    uint16_t info;
    char name[1];
} type_desc_t;

typedef struct {
    src_loc_t loc;
    const type_desc_t* array_type;
    const type_desc_t* index_type;
} out_of_bounds_t;

typedef struct {
    src_loc_t loc;
    const type_desc_t* lhs_type;
    const type_desc_t* rhs_type;
} shift_out_of_bounds_t;

typedef struct {
    src_loc_t loc;
    src_loc_t attr_loc;
    int arg_index;
} nonnull_arg_t;

typedef struct {
    src_loc_t loc;
    const type_desc_t* type;
} type_data_t;

typedef struct {
    src_loc_t loc;
    type_desc_t* type;
    uint8_t alignment;
    uint8_t type_check_kind;
} type_mismatch_t;

typedef type_data_t overflow_t;
typedef type_data_t invalid_value_t;
typedef type_data_t vla_bound_t;

static void handle_overflow(overflow_t* data,
                            unsigned long lhs,
                            unsigned long rhs,
                            char op) {
    panic("%s:%lu: integer overflow\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_add_overflow(overflow_t* data,
                                 unsigned long a,
                                 unsigned long b) {
    handle_overflow(data, a, b, '+');
}

void __ubsan_handle_sub_overflow(overflow_t* data,
                                 unsigned long a,
                                 unsigned long b) {
    handle_overflow(data, a, b, '-');
}

void __ubsan_handle_mul_overflow(overflow_t* data,
                                 unsigned long a,
                                 unsigned long b) {
    handle_overflow(data, a, b, '*');
}

void __ubsan_handle_negate_overflow(overflow_t* data, unsigned long a) {
    panic("%s:%lu: negate overflow\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_divrem_overflow(overflow_t* data,
                                    unsigned long a,
                                    unsigned long b) {
    panic("%s:%lu: divrem overflow\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_shift_out_of_bounds(shift_out_of_bounds_t* data,
                                        unsigned long a,
                                        unsigned long b) {
    panic("%s:%lu: shift out of bounds\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_type_mismatch(type_mismatch_t* data, unsigned long addr) {
    panic("%s:%lu: type mismatch: %lx\n", data->loc.file, data->loc.line, addr);
}

void __ubsan_handle_type_mismatch_v1(type_mismatch_t* data,
                                     unsigned long addr) {
    panic("%s:%lu: type mismatch v1: %lx, %d, %d\n", data->loc.file,
          data->loc.line, addr, data->type_check_kind, data->alignment);
}

void __ubsan_handle_out_of_bounds(out_of_bounds_t* data, unsigned long index) {
    panic("%s:%lu: out of bounds\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_builtin_unreachable(src_loc_t* loc) {
    panic("%s:%lu: unreachable\n", loc->file, loc->line);
}

void __ubsan_handle_missing_return(src_loc_t* loc) {
    panic("%s:%lu: missing return\n", loc->file, loc->line);
}

void __ubsan_handle_vla_bound_not_positive(vla_bound_t* data,
                                           unsigned long bound) {
    panic("%s:%lu: vla bound not positive\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_load_invalid_value(invalid_value_t* data,
                                       unsigned long val) {
    panic("%s:%lu: load invalid value\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_nonnull_arg(nonnull_arg_t* data) {
    panic("%s:%lu: nonnull arg\n", data->loc.file, data->loc.line);
}

void __ubsan_handle_nonnull_return(src_loc_t* loc) {
    panic("%s:%lu: nonnull return\n", loc->file, loc->line);
}

void __ubsan_handle_pointer_overflow(src_loc_t* loc,
                                     uintptr_t base,
                                     uintptr_t result) {
    panic("%s:%lu: pointer overflow\n", loc->file, loc->line);
}

// each entry is 1 if alloced, 0 otherwise
static bool asan_pagemap[MEMSIZE_PHYSICAL / PAGESIZE];

static bool asan = false;

static bool in_range(uintptr_t ka, char* start, char* end) {
    return ka >= (uintptr_t) start && ka < (uintptr_t) end;
}

static void asan_access(uintptr_t ka, size_t sz, bool write) {
    if (!asan || ka >= MEMSIZE_PHYSICAL) {
        return;
    }

    extern char _ktext_start, _ktext_end;
    if (write && in_range(ka, &_ktext_start, &_ktext_end)) {
        panic("write to %x: in kernel text!\n", ka);
    }
    extern char _krodata_start, _krodata_end;
    if (write && in_range(ka, &_krodata_start, &_krodata_end)) {
        panic("write to %x: in kernel rodata!\n", ka);
    }

    uintptr_t pn = pagenum(ka2pa(ka));
    if (!asan_pagemap[pn]) {
        panic("invalid access at %x\n", ka);
    }
}

void asan_mark_memory(uintptr_t pa, size_t sz, bool alloced) {
    uintptr_t pn = pagenum(pa);
    asan_pagemap[pn] = alloced;
}

static void asan_init() {
    extern char _kheap_start;
    uintptr_t heap_start = (uintptr_t) &_kheap_start;
    for (uintptr_t pa = 0; pa < MEMSIZE_PHYSICAL; pa += PAGESIZE) {
        uintptr_t pn = pagenum(pa);
        // all pages below heap are alloced
        asan_pagemap[pn] = pa < heap_start;
    }
}

void __asan_load1_noabort(unsigned long addr) {
    asan_access(addr, 1, false);
}
void __asan_load2_noabort(unsigned long addr) {
    asan_access(addr, 2, false);
}
void __asan_load4_noabort(unsigned long addr) {
    asan_access(addr, 4, false);
}
void __asan_load8_noabort(unsigned long addr) {
    asan_access(addr, 8, false);
}
void __asan_loadN_noabort(unsigned long addr, size_t sz) {
    asan_access(addr, sz, false);
}

void __asan_store1_noabort(unsigned long addr) {
    asan_access(addr, 1, true);
}
void __asan_store2_noabort(unsigned long addr) {
    asan_access(addr, 2, true);
}
void __asan_store4_noabort(unsigned long addr) {
    asan_access(addr, 4, true);
}
void __asan_store8_noabort(unsigned long addr) {
    asan_access(addr, 8, true);
}
void __asan_storeN_noabort(unsigned long addr, size_t sz) {
    asan_access(addr, sz, true);
}

void __asan_handle_no_return() {}
void __asan_before_dynamic_init(const char* module_name) {}
void __asan_after_dynamic_init() {}

void asan_enable() {
    asan_init();
    asan = true;
}

#endif
