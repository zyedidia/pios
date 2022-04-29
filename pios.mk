MEMMAP = $(PIOS)/memmap.ld

PIOS_CSRC = $(wildcard $(PIOS)/*.c) $(wildcard $(PIOS)/libc/*.c)
PIOS_HSRC = $(wildcard $(PIOS)/*.h) $(wildcard $(PIOS)/libc/*.h)
PIOS_SSRC = $(wildcard $(PIOS)/*.s)

PIOS_OBJ = $(PIOS_CSRC:.c=.o) $(PIOS_SSRC:.s=.o)
