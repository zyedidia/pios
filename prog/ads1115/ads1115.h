#pragma once

#include <stdint.h>

enum {
    conversion_reg = 0,
    config_reg = 1,
};

void ads1115_reset();
void ads1115_write(uint8_t dev_addr, uint8_t reg, uint16_t v);
uint16_t ads1115_read(uint8_t dev_addr, uint8_t reg);
uint8_t ads1115_config();
