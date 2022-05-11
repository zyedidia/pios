KERN = $(PIOS)/kern

MEMMAP = $(KERN)/memmap.ld

KERN_CSRC = $(wildcard $(KERN)/*.c) $(wildcard $(PIOS)/libc/*.c)
KERN_HSRC = $(wildcard $(KERN)/*.h) $(wildcard $(PIOS)/libc/*.h)
KERN_SSRC = $(wildcard $(KERN)/*.s)

KERN_OBJ_NOSAN = $(KERN)/ksan.o $(KERN)/boot.o $(KERN)/uart.o $(KERN)/start.o $(KERN)/gpio.o $(KERN)/kmalloc.o $(PIOS)/libc/tinyprintf.o

KERN_OBJ = $(KERN)/boot.o            \
		   $(KERN)/start.o           \
		   $(KERN)/uart.o            \
		   $(KERN)/gpio.o            \
		   $(KERN)/kern.o            \
		   $(KERN)/interrupts-asm.o  \
		   $(KERN)/interrupts.o      \
		   $(KERN)/syscall.o         \
		   $(KERN)/kmalloc.o         \
		   $(KERN)/ksan.o            \
		   $(PIOS)/libc/libc.o       \
		   $(PIOS)/libc/tinyprintf.o
