#include "pios.h"

void unreachable() {
    printf("should panic due to unreachable\n");

    __builtin_unreachable(); // should panic
}

static volatile int g;
void overflow() {
    printf("should panic due to integer overflow\n");

    int s = 2;
    for (int i = 0; i < 100; i++) {
        s = s * s; // should panic
    }
    // use result to prevent optimization
    g = s;
}

void use_after_free() {
    printf("should panic in heap\n");

    int* p = (int*) kmalloc(sizeof(int));
    *p = 42;
    kfree(p);
    g = *p; // should panic
}

void illegal_access() {
    printf("should panic in heap\n");

    int* p = (int*) kmalloc(sizeof(int) * 4);
    memset(p, 0, 4);
    g = p[4]; // should panic
    kfree(p);
}

void legal_access() {
    printf("should not panic\n");

    int* p = (int*) kmalloc(sizeof(int) * 4);
    memset(p, 0, 4);
    g = p[3]; // should not panic
    kfree(p);
}

void invalid_code() {
    printf("should panic on code segment\n");

    memset(invalid_code, 0, 8); // should panic
}

int main() {
#if (TEST == 1)
    unreachable();
#elif (TEST == 2)
    use_after_free();
#elif (TEST == 3)
    invalid_code();
#elif (TEST == 4)
    illegal_access();
#elif (TEST == 5)
    legal_access();
#elif (TEST == 6)
    overflow();
#endif

    return 0;
}
