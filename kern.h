#pragma once

#include "libc/tinyprintf.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

#include "kalloc.h"

void kernel_start();
void reboot();
