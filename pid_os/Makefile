include ../defs.mk

CFLAGS += -MMD

MEMMAP = memmap.ld
INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

OBJ = pidos.o start.o uart.o gpio.o ../libc/tinyprintf.o

all: pidos.bin

pidos.elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(PIOS)/kern/p-pidos.o: pidos.bin
	$(LD) -r -b binary -o $@ $<

exostall: $(PIOS)/kern/p-pidos.o

clean:
	rm -f *.d *.list *.o *.elf *.bin

-include $(DEP)

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
