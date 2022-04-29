#include "kern.h"

void kernel_start() {
    asan_enable();

    printf("kernel booted!\n");
    return;
}
