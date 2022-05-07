#include "pios.h"

extern int main();

void kernel_start() {
#if (SANITIZE == 1)
    asan_enable();
#endif
    cache_enable();

    main();
    return;
}
