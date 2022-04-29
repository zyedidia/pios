#include "pios.h"

void unreachable() {
    printf("unreachable: should panic with sanitizers:\n");
    __builtin_unreachable(); // should panic
}

void invalid_heap() {
    int* p = (int*) kmalloc(sizeof(int));
    *p = 42;
    kfree(p);
    *p = 100; // should panic
}

void invalid_code() {
    memset(invalid_code, 0, 8); // should panic
}

int main() {
    invalid_code();
    return 0;
}
