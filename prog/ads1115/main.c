#include "pios.h"
#include "ads1115.h"

int main() {
    i2c_init();

    uint8_t dev_addr = ads1115_config();

    for (unsigned i = 0; i < 50; i++) {
        int16_t v = ads1115_read(dev_addr, conversion_reg);
        printf("read=%d\n", v);
        delay_ms(250);
    }

    return 0;
}
