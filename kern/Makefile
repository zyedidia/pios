include kern.mk
include ../defs.mk

O ?= s

CFLAGS += -MMD

INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

all: $(KERN_OBJ)

kern.elf: $(KERN_OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.d *.list *.o *.elf *.bin

-include $(DEP)

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
