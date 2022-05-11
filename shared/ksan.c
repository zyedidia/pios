#include <stdbool.h>
#include <stdint.h>

#include "pios.h"

#if (SANITIZE == 1)

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

void __ubsan_handle_type_mismatch() {
    panic("type mismatch\n");
}

void __ubsan_handle_type_mismatch_v1() {
    panic("type mismatch v1\n");
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

#endif
