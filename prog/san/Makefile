SANITIZE = 1

include ../../pios.mk
include ../../defs.mk

O ?= s
TEST ?= 1
TESTS = 1 2 3 4 5 6
PROGS = $(addprefix test-,$(TESTS))

SRC = san.c

CFLAGS += -I$(PIOS)

all: $(addsuffix .bin,$(PROGS)) $(addsuffix .elf,$(PROGS))

install: test-$(TEST).bin
	pi-install $<

san-%.o: $(SRC)
	$(CC) $(CFLAGS) -DTEST=$* $(ASAN_FLAGS) $< -c -o $@

test-%.elf: $(PIOS_OBJ) san-%.o
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.list *.o *.elf *.bin

format:
	clang-format -i $(SRC)

qemu: test-$(TEST).elf
	$(QEMU) -M $(BOARD) -nographic -kernel $< -serial null -serial mon:stdio -no-reboot

.PHONY: format clean all install

