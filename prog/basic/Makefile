include ../../defs.mk

PROG = basic
MEMMAP = $(PROG).ld

exostall: $(PIOS)/kern/p-$(PROG).o

$(PROG).elf: $(MEMMAP) $(PROG).o
	$(LD) $(LDFLAGS) $(filter-out $<,$^) $(LDLIBS) -o $@

$(PIOS)/kern/p-$(PROG).o: $(PROG).bin
	$(LD) -r -b binary -o $@ $<

.PHONY: exostall
