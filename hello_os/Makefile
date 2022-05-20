include ../defs.mk

CFLAGS += -MMD

MEMMAP = memmap.ld
INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

OBJ = hello.o start.o uart.o gpio.o ../libc/tinyprintf.o

all: hello.bin

hello.elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(PIOS)/kern/p-hello.o: hello.bin
	$(LD) -r -b binary -o $@ $<

exostall: $(PIOS)/kern/p-hello.o

clean:
	rm -f *.d *.list *.o *.elf *.bin

-include $(DEP)

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
