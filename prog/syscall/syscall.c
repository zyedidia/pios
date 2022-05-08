#include "pios.h"
#include "u-syscall.h"

int main() {
    irq_init_table();
    exit();
    return 0;
}
