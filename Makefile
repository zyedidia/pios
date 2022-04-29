include defs.mk
include pios.mk

O ?= s

KERN = pios

all: $(KERN).bin

install: $(KERN).bin
	pi-install $(KERN).bin

$(KERN).elf: $(PIOS_OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.list *.o $(KERN).elf $(KERN).bin

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
