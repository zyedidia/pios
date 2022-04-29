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

PIOS ?= $(shell git rev-parse --show-toplevel)

CFLAGS = -O$(O) -g -Wall -nostdlib -nostartfiles -ffreestanding -Wa,-mcpu=$(CPU) -Wa,-march=$(ARCH)
ifeq ($(SANITIZE),1)
	CFLAGS += -fsanitize=undefined -fsanitize=kernel-address
endif

ASFLAGS = -mcpu=$(CPU) -march=$(ARCH)
LDFLAGS = -T $(MEMMAP)
LDLIBS = $(LIBGCC)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.s
	$(CPP) $< | $(AS) $(ASFLAGS) -c -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@
