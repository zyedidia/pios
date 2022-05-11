MEMMAP = $(PIOS)/memmap.ld

EXO_CSRC = $(wildcard $(PIOS)/exo/*.c) $(wildcard $(PIOS)/libc/*.c)
EXO_HSRC = $(wildcard $(PIOS)/exo/*.h) $(wildcard $(PIOS)/libc/*.h)
EXO_SSRC = $(wildcard $(PIOS)/exo/*.s)

EXO_OBJ_NOSAN = $(PIOS)/uart.o $(PIOS)/ksan.o $(PIOS)/cstart.o $(PIOS)/start.o $(PIOS)/gpio.o $(PIOS)/kmalloc.o $(PIOS)/libc/tinyprintf.o
EXO_OBJ = $(EXO_CSRC:.c=.o) $(EXO_SSRC:.s=.o)
