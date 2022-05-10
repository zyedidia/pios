#pragma once

#include <stdint.h>

void i2c_init();
int i2c_write(uint8_t addr, uint8_t* data, uint16_t nbytes);
int i2c_read(uint8_t addr, uint8_t* data, uint16_t nbytes);
