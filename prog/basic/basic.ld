SECTIONS
{
    .text 0x8000 :  { 
        KEEP(*(.text.boot))  
        *(.text*) 
        . = ALIGN(8);
    }
    .rodata : { *(.rodata*) }
    .data : { 
            *(.data*) 
            . = ALIGN(4);
    } 
    .bss : {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        . = ALIGN(8);
    }
}
