include ../../pios.mk
include ../../defs.mk

O ?= s

PROG = irq

CSRC = $(wildcard *.c)
HSRC = $(wildcard *.h)

OBJ = $(CSRC:.c=.o) $(PIOS_OBJ)

CFLAGS += -I$(PIOS)

all: $(PROG).bin

install: $(PROG).bin
	pi-install $<

$(PROG).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.list *.o $(PROG).elf $(PROG).bin

format:
	clang-format -i $(CSRC) $(HSRC)

qemu: $(PROG).elf
	qemu-system-arm -M raspi1ap -plugin $(PIOS)/plugin/memtrace/libmemtrace.so -nographic -d plugin -D trace.txt -kernel $< -serial null -serial mon:stdio -no-reboot

.PHONY: format clean all install qemu
