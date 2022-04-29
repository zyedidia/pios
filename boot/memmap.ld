SECTIONS
{
    .boot 0x8000 : {
        KEEP(*(.boot))
        . = ALIGN(8);
    }

    .text 0x10000 :  { 
        *(.text*) 
        . = ALIGN(8);
    }
    .rodata : { *(.rodata*) }
    .data : { 
            __data_start__ = .;
            *(.data*) 
            . = ALIGN(4);
            __data_end__ = .;
    } 
    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        . = ALIGN(8);
        __prog_end__ = .;
        __heap_start__ = .;
    }
}
