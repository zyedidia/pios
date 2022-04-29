#include <stdbool.h>
#include <stdint.h>

#include "bits.h"
#include "dev.h"
#include "gpio.h"
#include "uart.h"

typedef struct {
    uint32_t io;
    uint32_t ier;
    uint32_t iir;
    uint32_t lcr;
    uint32_t mcr;
    uint32_t lsr;
    uint32_t msr;
    uint32_t scratch;
    uint32_t cntl;
    uint32_t stat;
    uint32_t baud;
} aux_periphs_t;

#define ENABLE_UART 1
#define RX_ENABLE (1 << 0)
#define TX_ENABLE (1 << 1)
#define CLEAR_TX_FIFO (1 << 1)
#define CLEAR_RX_FIFO (1 << 2)
#define CLEAR_FIFOS (CLEAR_TX_FIFO | CLEAR_RX_FIFO)
#define IIR_RESET ((0b11 << 6) | 1)

static volatile uint32_t* const aux_enables = (uint32_t*) 0x20215004;
static volatile aux_periphs_t* const uart = (aux_periphs_t*) 0x20215040;

void uart_init(unsigned baud) {
    gpio_set_func(GPIO_TX, GPIO_FUNC_ALT5);
    gpio_set_func(GPIO_RX, GPIO_FUNC_ALT5);

    dev_barrier();

    *aux_enables = *aux_enables | ENABLE_UART;

    dev_barrier();

    uart->cntl = 0;
    uart->ier = 0;
    uart->lcr = 0b11;
    uart->mcr = 0;
    uart->iir = IIR_RESET | CLEAR_FIFOS;
    uart->baud = (250 * 1000 * 1000) / (baud * 8) - 1;
    uart->cntl = RX_ENABLE | TX_ENABLE;

    dev_barrier();
}

bool uart_rx_empty() {
    return bit_get(uart->stat, 0) == 0;
}

unsigned uart_rx_sz() {
    return bits_get(uart->stat, 16, 19);
}

bool uart_can_tx() {
    return bit_get(uart->stat, 1) != 0;
}

uint8_t uart_rx() {
    dev_barrier();
    while (uart_rx_empty())
        ;
    uint8_t c = uart->io & 0xff;
    dev_barrier();
    return c;
}

void uart_tx(uint8_t c) {
    dev_barrier();
    while (!uart_can_tx())
        ;
    uart->io = c & 0xff;
    dev_barrier();
}

void uart_putc(void* p, char c) {
    (void) p;
    uart_tx(c);
}

bool uart_tx_empty() {
    dev_barrier();

    // broadcom p 18: "This bit (9) is set if the transmitter is idle and the
    // transmit FIFO is empty."
    // transmitter done: idle and empty
    return bit_get(uart->stat, 9) == 1;
}

void uart_tx_flush() {
    while (!uart_tx_empty())
        ;
}
