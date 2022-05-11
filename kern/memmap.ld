SECTIONS
{
    .text 0x80008000 :  {
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
}
