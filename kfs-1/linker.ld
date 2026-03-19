OUTPUT_FORMAT("elf32-i386") ;
ENTRY(start) ;                  /* Entry symbol in boot.asm */

SECTIONS
{
    /* Code section at 0x100000 1MB */
    . = 0x00100000 ;

    .multiboot : {
        *(.multiboot) ;         /* Multiboot2 header*/
    }

    .text : {
        *(.text) ;              /* Code */
    }

    .rodata : {
        *(.rodata) ;
    }

    .data : {
        *(.data) ;
    }

    .bss (NOLOAD) : {
        *(.bss) ; 
    }
}

