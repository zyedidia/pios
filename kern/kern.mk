MEMMAP = $(PIOS)/memmap.ld

KERN = $(PIOS)/kern

KERN_CSRC = $(wildcard $(KERN)/*.c) $(wildcard $(PIOS)/libc/*.c)
KERN_HSRC = $(wildcard $(KERN)/*.h) $(wildcard $(PIOS)/libc/*.h)
KERN_SSRC = $(wildcard $(KERN)/*.s)

KERN_OBJ_NOSAN = $(KERN)/uart.o $(KERN)/ksan.o $(KERN)/cstart.o $(KERN)/start.o $(KERN)/gpio.o $(KERN)/kmalloc.o $(KERN)/libc/tinyprintf.o
KERN_OBJ = $(KERN_CSRC:.c=.o) $(KERN_SSRC:.s=.o)
