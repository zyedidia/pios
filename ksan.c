#include <stdint.h>
#include <stdbool.h>

#include "kern.h"

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

typedef type_data_t overflow_t;
typedef type_data_t invalid_value_t;
typedef type_data_t vla_bound_t;

static void handle_overflow(overflow_t* data,
                            unsigned long lhs,
                            unsigned long rhs,
                            char op) {
    printf("SAN: %s:%lu: integer overflow\n", data->loc.file, data->loc.line);
    panic("dead");
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
    panic("negate overflow\n");
}

void __ubsan_handle_divrem_overflow(overflow_t* data,
                                    unsigned long a,
                                    unsigned long b) {
    panic("divrem overflow\n");
}

void __ubsan_handle_shift_out_of_bounds(shift_out_of_bounds_t* data,
                                        unsigned long a,
                                        unsigned long b) {
    panic("shift out of bounds\n");
}

void __ubsan_handle_type_mismatch() {
    panic("type mismatch\n");
}

void __ubsan_handle_type_mismatch_v1() {
    panic("type mismatch v1\n");
}

void __ubsan_handle_out_of_bounds(out_of_bounds_t* data, unsigned long index) {
    panic("out of bounds\n");
}

void __ubsan_handle_builtin_unreachable(src_loc_t* location) {
    panic("unreachable\n");
}

void __ubsan_handle_missing_return(src_loc_t* location) {
    panic("missing return\n");
}

void __ubsan_handle_vla_bound_not_positive(vla_bound_t* data,
                                           unsigned long bound) {
    panic("vla bound not positive\n");
}

void __ubsan_handle_load_invalid_value(invalid_value_t* data,
                                       unsigned long val) {
    panic("load invalid value\n");
}

void __ubsan_handle_nonnull_arg(nonnull_arg_t* data) {
    panic("nonnull arg\n");
}

void __ubsan_handle_nonnull_return(src_loc_t* location) {
    panic("nonnull return\n");
}

void __ubsan_handle_pointer_overflow(src_loc_t* location,
                                     uintptr_t base,
                                     uintptr_t result) {
    panic("pointer overflow\n");
}

static bool asan = false;

void asan_access(unsigned long addr, size_t sz, bool write) {
    /* if (asan) { */
    /*     printf("access at 0x%lx\n", addr); */
    /* } */
    extern char __code_start__, __code_end__;
    if (write && addr >= (uintptr_t) &__code_start__ && addr < (uintptr_t) &__code_end__) {
        panic("attempt to write code segment\n");
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

void __asan_handle_no_return() {
}
void __asan_before_dynamic_init(const char* module_name) {
}
void __asan_after_dynamic_init() {
}

void asan_enable() {
    asan = true;
}
