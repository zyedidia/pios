PREFIX = arm-none-eabi

CC = $(PREFIX)-gcc
AS = $(PREFIX)-as
LD = $(PREFIX)-ld
CPP = $(PREFIX)-cpp
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump

LIBGCC = $(shell $(CC) --print-file-name=libgcc.a)

CPU = arm1176jzf-s
ARCH = armv6zk

MEMMAP = memmap.ld
O ?= s
CFLAGS = -O$(O) -g -Wall -nostdlib -nostartfiles -ffreestanding -Wa,-mcpu=$(CPU) -Wa,-march=$(ARCH)
ASFLAGS = -mcpu=$(CPU) -march=$(ARCH)
LDFLAGS = -T $(MEMMAP)
LDLIBS = $(LIBGCC)

KERN=pios

CSRC = $(wildcard *.c) $(wildcard libc/*.c)
HSRC = $(wildcard *.h) $(wildcard libc/*.h)
SSRC = $(wildcard *.s)

OBJ = $(CSRC:.c=.o) $(SSRC:.s=.o)

all: $(KERN).bin

install: $(KERN).bin
	pi-install $(KERN).bin

$(KERN).elf: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.s
	$(CPP) $< | $(AS) $(ASFLAGS) -c -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@

format:
	clang-format -i $(CSRC) $(HSRC)

clean:
	rm -f *.list *.o $(KERN).elf $(KERN).bin

.PHONY: format all install
