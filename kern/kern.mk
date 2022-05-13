KERN = $(PIOS)/kern

MEMMAP = $(KERN)/memmap.ld

KERN_CSRC = $(wildcard $(KERN)/*.c) $(wildcard $(PIOS)/libc/*.c)
KERN_HSRC = $(wildcard $(KERN)/*.h) $(wildcard $(PIOS)/libc/*.h)
KERN_SSRC = $(wildcard $(KERN)/*.s)

KERN_OBJ_NOSAN = $(KERN)/ksan.o $(KERN)/boot.o $(KERN)/uart.o $(KERN)/start.o $(KERN)/gpio.o $(KERN)/kmalloc.o $(PIOS)/libc/tinyprintf.o
KERN_OBJ = $(KERN_CSRC:.c=.o) $(KERN_SSRC:.s=.o) $(wildcard p-*.o)
