#pragma once

#include "libc/tinyprintf.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

void kernel_start();
void reboot();
