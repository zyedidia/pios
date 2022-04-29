include ../defs.mk

MEMMAP = memmap.ld

O ?= s

KERN = boot

CSRC = $(wildcard *.c)
HSRC = $(wildcard *.h)

LIBOBJ = start.o uart.o gpio.o
OBJ = $(CSRC:.c=.o) $(addprefix $(PIOS)/,$(LIBOBJ))

CFLAGS += -I$(PIOS)

all: $(KERN).bin

$(KERN).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.list *.o $(KERN).elf $(KERN).bin

format:
	clang-format -i $(CSRC) $(HSRC)

.PHONY: format all install
