#include "pios.h"
#include "ads1115.h"

void ads1115_reset() {
    uint8_t rst = 0x6;
    i2c_write(0, &rst, 1);
}

void ads1115_write(uint8_t dev_addr, uint8_t reg, uint16_t v) {
    uint8_t data[3] = {reg, v >> 8, v & 0xff};
    i2c_write(dev_addr, data, 3);
}

uint16_t ads1115_read(uint8_t dev_addr, uint8_t reg) {
    i2c_write(dev_addr, &reg, 1);
    uint8_t data[2];
    i2c_read(dev_addr, data, 2);
    return ((uint16_t) data[0] << 8) | data[1];
}

uint8_t ads1115_config() {
    uint8_t dev_addr = 0b1001000;

    ads1115_reset();
    delay_us(25);

    uint16_t c = ads1115_read(dev_addr, config_reg);
    c = bits_set(c, 8, 8, 0); // MODE
    c = bits_set(c, 5, 7, 0b111); // DR
    c = bits_set(c, 9, 11, 0b001); // PGA
    c = bits_set(c, 12, 14, 0b100); // MUX
    ads1115_write(dev_addr, config_reg, c);

    return dev_addr;
}
