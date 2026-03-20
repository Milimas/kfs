# Makefile -- kernal Makefile
AS = nasm
AFLAGS = -f elf32 -g -F dwarf
CFLAGS = -m32 -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs -g
LDFLAGS = -m elf_i386
LD = ld
CC = gcc

all: kernel.elf

boot.o: boot.asm
	$(AS) $(AFLAGS) boot.asm -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.elf: boot.o kernel.o
	$(LD) $(LDFLAGS) -T linker.ld boot.o kernel.o -o kernel.elf

.PHONY: iso run clean
iso: kernel.elf
	cp kernel.elf iso/boot/
	grub-mkrescue -o kfs.iso iso

run: iso
	qemu-system-i386 -cdrom kfs.iso

rdebug:
	qemu-system-i386 -cdrom kfs.iso -S -s
debug:
	gdb -x gdb.cfg kernel.elf

clean:
	rm -f boot.o kernel.o kernel.elf kfs.iso iso/boot/kernel.elf

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build kernel.elf (default)"
	@echo "  iso      - Build bootable ISO"
	@echo "  run      - Build and run in QEMU"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this message"

.PHONY: all iso run clean help

