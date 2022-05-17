include ../defs.mk

CFLAGS += -MMD

MEMMAP = memmap.ld
INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

OBJ = shell.o start.o uart.o gpio.o ../libc/tinyprintf.o

all: shell.bin

shell.elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(PIOS)/kern/p-shell.o: shell.bin
	$(LD) -r -b binary -o $@ $<

exostall: $(PIOS)/kern/p-shell.o

clean:
	rm -f *.d *.list *.o *.elf *.bin

TTY = /dev/ttyUSB0
BAUD = 115200

screen:
	screen $(TTY) $(BAUD)

-include $(DEP)

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install screen

# make install
# CTRL+C
# screen /dev/cu.usbserial-0001 115200
# CTRL+A CTRL+\ y
