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
    dw 0
    dw 0
    dw 8
header_end:


section .text
global start
start:
    ; ! Do not zero the segment registers in 32-bit protected mode after multiboot,
    ; The segments are already set up by GRUB Zeroing them can cause issues.
    mov esp, stack_top
    
    call vga_clear          ; clear VGA text buffer
    push STARTING_KERNEL
    call vga_print
    add esp, 4              ; clean up stack argument

    ; push args in cdecl order (right to left)
    push ebx,               ; multiboot_info
    push eax                ; magic number
    call kmain

    call vga_clear          ; clear VGA text buffer
    push HALT_MESSAGE
    call vga_print
    add esp, 4

    jmp halt


extern kmain

; vga_clear -- clears the sreen
vga_clear:
    push edi
    push ecx

    mov edi, 0xB8000        ; VGA address
    mov ecx, 2000           ; 80 * 25 cells
    mov ax, 0x0F20          ; space (0x20) with white on black attribute (0x0F)
    rep stosw               ; write 2000 words

    pop ecx
    pop edi
    ret


; vga_print -- prints a null-terminated string to VGA text buffer
; arg: [esp + 4] = pointer to string
vga_print:
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 8]      ; string pointer
    mov edi, 0xB8000        ; VGA text buffer
    mov ah, 0x0F            ; attribute: white on black


.loop:
    lodsb                   ; load byte from [esi] into al
    test al, al             ; check for null terminator
    jz .done
    stosb                   ; write character to [edi]
    mov [edi], ah           ; write attribute byte
    inc edi                 ; skip attribute byte
    jmp .loop


.done:
    pop edi
    pop esi
    pop ebp
    ret


halt:
    cli
    hlt
    jmp halt


section .data
align 8
STARTING_KERNEL db "Starting Kernel", 0
HALT_MESSAGE DB "HALTING..."


section .bss
align 16
stack_bottom:
    resb 16384               ; 16 KB stack
stack_top:

