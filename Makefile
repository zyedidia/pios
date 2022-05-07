include pios.mk
include defs.mk

O ?= s

all: $(PIOS_OBJ)

clean:
	rm -f *.list *.o $(KERN).elf $(KERN).bin build

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
