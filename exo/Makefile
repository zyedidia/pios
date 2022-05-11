include exo.mk
include ../defs.mk

O ?= s

CFLAGS += -MMD
CFLAGS += -I$(PIOS)
ASFLAGS += -I$(PIOS)
CPPFLAGS += -I$(PIOS)
DEP = $(EXO_OBJ:.o=.d)

all: $(EXO_OBJ)

clean:
	rm -f *.list *.o *.elf *.bin

-include $(DEP)

format:
	clang-format -i $(PIOS_CSRC) $(PIOS_HSRC)

.PHONY: format all install
