#include "pios.h"

extern int main();

void _enable_interrupts();

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif
    cache_enable();

    main();
    return;
}
