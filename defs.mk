PREFIX = arm-none-eabi

CC = $(PREFIX)-gcc
AS = $(PREFIX)-as
LD = $(PREFIX)-ld
CPP = $(PREFIX)-cpp
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump

QEMU = qemu-system-arm
BOARD = raspi1ap
GDB = gdb-multiarch

LIBGCC = $(shell $(CC) --print-file-name=libgcc.a)

CPU = arm1176jzf-s
ARCH = armv6zk

PIOS ?= $(shell git rev-parse --show-toplevel)
BUILDDIR = $(PIOS)/build

CFLAGS = -O$(O) -g -Wall -nostdlib -nostartfiles -ffreestanding -Wa,-mcpu=$(CPU) -Wa,-march=$(ARCH)
ASFLAGS = -mcpu=$(CPU) -march=$(ARCH)
LDFLAGS = -T $(MEMMAP)
LDLIBS = $(LIBGCC)
SANITIZE ?= 0
ifeq ($(SANITIZE),1)
	ASAN_FLAGS = -fsanitize=kernel-address,undefined -DSANITIZE=1
else
	ASAN_FLAGS = -DSANITIZE=0
endif

$(PIOS_OBJ_NOSAN): ASAN_FLAGS := -DSANITIZE=$(SANITIZE)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $< -c -o $@

$(BUILDDIR)/%.o: %.s
	@mkdir -p $(@D)
	$(CPP) $< | $(AS) $(ASFLAGS) -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $< -c -o $@

%.o: %.s
	$(CPP) $< | $(AS) $(ASFLAGS) -c -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@
