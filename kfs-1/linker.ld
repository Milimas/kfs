/* linker.ld -- kernal linker file */
OUTPUT_FORMAT("elf32-i386") ;
ENTRY(start) ;                  /* Entry symbol in boot.asm */

PHDRS
{
    text PT_LOAD FLAGS(5) ; /* R + X */
    data PT_LOAD FLAGS(6) ; /* R + W */
}

SECTIONS
{
    /* Code section at 0x200000 2MB */
    . = 0x00200000 ;

    /* Multiboot2 header*/
    .multiboot : {  *(.multiboot) } :text
    /* Code */
    .text : { *(.text) } :text
    .rodata : { *(.rodata) } :text

    .data : { *(.data) } :data
    /* NOLOAD section exists in memory but not in file 
     * MemSiz includes .bss
     * FileSiz does not
     * .bss Stack memory
     */
    .bss (NOLOAD) : { *(.bss) } :data
}

