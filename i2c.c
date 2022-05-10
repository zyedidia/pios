#include <stdint.h>

#include "dev.h"
#include "gpio.h"
#include "pios.h"

typedef struct {
    unsigned read     : 1;  // read transfer (0: write packet, 1: read packet)
    unsigned _unused0 : 3;
    unsigned clear    : 2;  // fifo clear (0b00: no action, 0bx1 or 0b1x: clear
                            // fifo, reads as 0)
    unsigned _unused1 : 1;
    unsigned st       : 1;  // start transfer
    unsigned intd     : 1;  // interrupt on done
    unsigned intt     : 1;  // interrupt on tx
    unsigned intr     : 1;  // interrupt on rx
    unsigned _unused2 : 4;
    unsigned i2cen    : 1;  // i2c enable
    unsigned _unused3 : 16;
} ctrl_reg_t;
_Static_assert(sizeof(ctrl_reg_t) == 4, "invalid size for ctrl_reg_t");

static ctrl_reg_t ctrl_clear(ctrl_reg_t c) {
    c._unused0 = 0;
    c._unused1 = 0;
    c._unused2 = 0;
    c._unused3 = 0;
    return c;
}

static ctrl_reg_t ctrl_read(volatile ctrl_reg_t* c) {
    return ctrl_clear(*c);
}

typedef struct {
    unsigned ta       : 1;  // transfer active
    unsigned done     : 1;  // transfer done
    unsigned txw      : 1;  // fifo needs writing
    unsigned rxr      : 1;  // fifo needs reading
    unsigned txd      : 1;  // fifo can accept data
    unsigned rxd      : 1;  // fifo contains data
    unsigned txe      : 1;  // fifo empty
    unsigned rxf      : 1;  // fifo full
    unsigned err      : 1;  // ack error
    unsigned clktk    : 1;  // clock stretch timeout
    unsigned _unused0 : 22;
} stat_reg_t;
_Static_assert(sizeof(stat_reg_t) == 4, "invalid size for stat_reg_t");

static stat_reg_t stat_clear(stat_reg_t s) {
    s._unused0 = 0;
    return s;
}

static stat_reg_t stat_read(volatile stat_reg_t* s) {
    return stat_clear(*s);
}

typedef struct {
    ctrl_reg_t ctrl;
    stat_reg_t status;

    uint32_t data_len;
    uint32_t dev_addr;
    uint32_t data_fifo;
    uint32_t clock_div;
    uint32_t clock_delay;
    uint32_t clock_stretch_timeout;
} i2c_regs_t;

static volatile i2c_regs_t* const i2c = (i2c_regs_t*) 0x20804000;  // BSC1

static void reset_status() {
    stat_reg_t s = stat_read(&i2c->status);
    // all are cleared by writing 1
    s.done = 1;
    s.err = 1;
    s.clktk = 1;
    i2c->status = s;
}

void i2c_init() {
    dev_barrier();
    gpio_set_func(GPIO_SDA, GPIO_FUNC_ALT0);
    gpio_set_func(GPIO_SCL, GPIO_FUNC_ALT0);
    dev_barrier();
    ctrl_reg_t ctrl;
    memset(&ctrl, 0, sizeof(ctrl_reg_t));
    ctrl.i2cen = 1;
    i2c->ctrl = ctrl;
    reset_status();
    dev_barrier();
}

#define wait_for(_cond)                                  \
    do {                                                 \
        while (!(_cond)) {                               \
            if (i2c->status.err)                         \
                panic("i2c slave ack error\n");          \
            if (i2c->status.clktk)                       \
                panic("i2c slave held line too long\n"); \
        }                                                \
    } while (0)

static void transfer_start(uint8_t addr, uint16_t nbytes, int read) {
    // wait for any previous transfer to complete
    wait_for(i2c->status.ta == 0);
    reset_status();

    i2c->dev_addr = addr;
    i2c->data_len = nbytes;

    ctrl_reg_t c = ctrl_read(&i2c->ctrl);
    c.clear = 0b11;
    c.st = 1;
    c.read = read;
    i2c->ctrl = c;

    // wait until transfer begins
    wait_for(i2c->status.ta == 1);
}

static void transfer_end() {
    wait_for(i2c->status.done == 1);
}

int i2c_write(uint8_t addr, uint8_t* data, uint16_t nbytes) {
    dev_barrier();
    transfer_start(addr, nbytes, 0);

    for (unsigned i = 0; i < nbytes; i++) {
        // wait for there to be more room
        wait_for(i2c->status.txd == 1);
        i2c->data_fifo = data[i];
    }

    transfer_end();
    dev_barrier();
    return 1;
}

int i2c_read(uint8_t addr, uint8_t* data, uint16_t nbytes) {
    dev_barrier();
    transfer_start(addr, nbytes, 1);

    for (unsigned i = 0; i < nbytes; i++) {
        // wait for at least one byte to be available
        wait_for(i2c->status.rxd == 1);
        data[i] = i2c->data_fifo & 0xff;
    }

    transfer_end();
    dev_barrier();
    return 1;
}
