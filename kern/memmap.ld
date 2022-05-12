ENTRY(_boot_start)

SECTIONS
{
    .text.boot 0x8000 : {
        _boot_start = .;
        KEEP(*(.text.start.boot))
        KEEP(*(.text.boot))
        _boot_end = .;
        . = ALIGN(8);
    }

    /* rest of kernel is at high addresses */
    . = . | (1 << 31);

    .text : AT (0x8180) {
        _ktext_start = .;
        KEEP(*(.boot))  
        *(.text*) 
        _ktext_end = .;
        . = ALIGN(8);
    }
    .rodata : {
        _krodata_start = .;
        *(.rodata*)
        _krodata_end = .;
    }
    .data : { 
        _kdata_start = .;
        *(.data*) 
        . = ALIGN(4);
        _kdata_end = .;
    } 

    .bss : {
        . = ALIGN(4);
        _kbss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _kbss_end = .;
        . = ALIGN(8);
        _kheap_start = .;
    }

    /DISCARD/ : { *(.eh_frame .note.GNU-stack) }
}
