; boot.asm -- multiboot2 specification implementation
[bits 32]


MAGIC           equ 0xE85250D6                          ; magic number for grub
ARCHITECTURE    equ 0                                   ; architecture 0 for i386
LENGTH          equ (header_end - multiboot_header)     ; header length
CHASUM          equ -(MAGIC + ARCHITECTURE + LENGTH)    ; checksum must equal zero sum when added to the header

section .multiboot
align 8
multiboot_header:
    dd MAGIC                ; magic
    dd ARCHITECTURE         ; architecture
    dd LENGTH               ; header length
    dd CHASUM               ; checksum

    ; framebuffer tag
    align 8
    dw 5                    ; tag type 5 = framebuffer
    dw 0                    ; flags
    dd 20                   ; size of this tag (bytes)
    dd 1024                 ; prefered width
    dd 768                  ; prefered height
    dd 32                   ; prefered bit depth (32 = ARGB/XRGB)

    ; end tag (required)
    align 8
    dw 0
    dw 0
    dd 8
header_end:


section .text
global start
start:
    ; ! Do not zero the segment registers in 32-bit protected mode after multiboot,
    ; The segments are already set up by GRUB Zeroing them can cause issues.
    mov esp, stack_top

    ; push args in cdecl order (right to left)
    push ebx                ; multiboot_info
    push eax                ; magic number
    call kmain
    add esp, 8              ; clean up stack argument

    jmp halt


extern kmain


halt:
    cli
    hlt
    jmp halt


section .data
align 8


section .bss
align 16
stack_bottom:
    resb 16384               ; 16 KB stack
stack_top:

