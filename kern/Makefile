include kern.mk
include ../defs.mk

O ?= s

CFLAGS += -MMD

INCLUDE = -I$(KERN) -I$(PIOS)
CFLAGS += $(INCLUDE)
ASFLAGS += $(INCLUDE)
CPPFLAGS += $(INCLUDE)
DEP = $(KERN_OBJ:.o=.d)

kern.elf: $(KERN_OBJ)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

install: kern.bin
	pi-install $<

clean:
	rm -f *.d *.list *.o *.elf *.bin

-include $(DEP)

qemu: kern.elf
	$(QEMU) -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot

qemu-gdb: kern.elf
	$(QEMU) -s -S -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot &
	$(GDB) -ex "file kern.elf" -ex "target remote localhost:1234"

format:
	clang-format -i $(KERN_CSRC) $(KERN_HSRC)

.PHONY: format all install
