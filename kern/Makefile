all: kern.bin

O ?= s

# these rules ensure dependencies are created
DEPCFLAGS = -MD -MF $(DEPSDIR)/$(notdir $*).d -MP
DEPSDIR := .deps
BUILDSTAMP := $(DEPSDIR)/rebuildstamp
DEPFILES := $(wildcard $(DEPSDIR)/*.d)
ifneq ($(DEPFILES),)
include $(DEPFILES)
endif

include kern.mk
include ../defs.mk

CFLAGS += $(DEPCFLAGS)

ifneq ($(RANDOMIZE),)
CFLAGS += -DRANDOMIZE_KMALLOC
endif

INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

# when the C compiler or optimization flags change, rebuild all objects
ifneq ($(strip $(DEP_CC)),$(strip $(CC) $(CFLAGS) $(ASAN_FLAGS) $(O)))
DEP_CC := $(shell mkdir -p $(DEPSDIR); echo >$(BUILDSTAMP); echo "DEP_CC:=$(CC) $(CFLAGS) $(ASAN_FLAGS) $(O)" >$(DEPSDIR)/_cc.d)
endif

kern.elf: $(MEMMAP) $(KERN_OBJ)
	$(LD) $(LDFLAGS) $(filter-out $<,$^) $(LDLIBS) -o $@

install: kern.bin
	pi-install $<

clean:
	rm -rf *.d *.list *.o *.elf *.bin .deps

-include $(DEP)

qemu: kern.elf
	$(QEMU) -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot

qemu-gdb: kern.elf
	$(QEMU) -s -S -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot &
	$(GDB) -ex "file kern.elf" -ex "target remote localhost:1234"

format:
	clang-format -i $(KERN_CSRC) $(KERN_HSRC)

p-basic.o:
	make -C $(PIOS)/prog/basic exostall

p-hello.o:
	make -C $(PIOS)/hello_os exostall

p-pidos.o:
	make -C $(PIOS)/pid_os exostall

.PHONY: format all install p-basic.o p-hello.o p-pidos.o
