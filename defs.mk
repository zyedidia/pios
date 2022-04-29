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
ASFLAGS = -mcpu=$(CPU) -march=$(ARCH)
LDFLAGS = -T $(MEMMAP)
LDLIBS = $(LIBGCC)
ifeq ($(SANITIZE),1)
	ASAN_FLAGS = -fsanitize=kernel-address,undefined
else
	ASAN_FLAGS =
endif

$(PIOS_OBJ_NOSAN): ASAN_FLAGS :=

%.o: %.c
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $< -c -o $@

%.o: %.s
	$(CPP) $< | $(AS) $(ASFLAGS) -c -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@
