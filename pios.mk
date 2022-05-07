MEMMAP = $(PIOS)/memmap.ld

PIOS_CSRC = $(wildcard $(PIOS)/*.c) $(wildcard $(PIOS)/libc/*.c)
PIOS_HSRC = $(wildcard $(PIOS)/*.h) $(wildcard $(PIOS)/libc/*.h)
PIOS_SSRC = $(wildcard $(PIOS)/*.s)

PIOS_OBJ_NOSAN = $(BUILDDIR)/uart.o 	\
		 $(BUILDDIR)/ksan.o 	\
		 $(BUILDDIR)/cstart.o 	\
		 $(BUILDDIR)/start.o 	\
		 $(BUILDDIR)/gpio.o 	\
		 $(BUILDDIR)/kmalloc.o 	\
		 $(BUILDDIR)/libc/tinyprintf.o

# NOTE[masot]: This will fail if any of the csrc, ssrc are not directly under
# root dir
PIOS_OBJ = $(PIOS_CSRC:$(PIOS)/%.c=$(BUILDDIR)/%.o) \
	   $(PIOS_SSRC:$(PIOS)/%.s=$(BUILDDIR)/%.o)
