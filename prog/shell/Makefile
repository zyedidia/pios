include ../../pios.mk
include ../../defs.mk

O ?= s

PROG = shell

CSRC = $(wildcard *.c)
HSRC = $(wildcard *.h)

OBJ = $(CSRC:.c=.o) $(PIOS_OBJ)

CFLAGS += -I$(PIOS)

TIMEOUT = 3
TTY = /dev/cu.usbserial-0001
BAUD = 115200

all: $(PROG).bin

install: $(PROG).bin
	pi-install $<

screen: $(PROG).bin
	screen $(TTY) $(BAUD)

$(PROG).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

include ../../common.mk

qemu: $(PROG).elf
	$(QEMU) -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot

qemu-gdb: $(PROG).elf
	$(QEMU) -s -S -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot &
	$(GDB) -ex "file $(PROG).elf" -ex "target remote localhost:1234"

.PHONY: format clean all install qemu

# make install
# CTRL+C
# screen /dev/cu.usbserial-0001 115200
# CTRL+A CTRL+\ y
