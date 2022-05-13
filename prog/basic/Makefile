include ../../defs.mk

PROG = basic
MEMMAP = $(PROG).ld

exostall: $(PIOS)/kern/p-$(PROG).c

$(PROG).elf: $(MEMMAP) $(PROG).o
	$(LD) $(LDFLAGS) $(filter-out $<,$^) $(LDLIBS) -o $@

$(PIOS)/kern/p-$(PROG).c: $(PROG).bin
	$(PIOS)/tools/exify/exify.sh $< > $@ $(PROG)

.PHONY: exostall
