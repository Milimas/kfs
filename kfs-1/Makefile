AS = nasm
AFLAGS = -f elf32
LDFLAGS = -m elf_i386
LD = ld

all: kernel.elf

boot.o: boot.asm
	$(AS) $(AFLAGS) boot.asm -o boot.o

kernel.elf: boot.o
	$(LD) $(LDFLAGS) -T linker.ld boot.o -o kernel.elf

.PHONY: iso run clean
iso: kernel.elf
	cp kernel.elf iso/boot/
	grub-mkrescue -o kfs.iso iso

run: iso
	qemu-system-i386 -cdrom kfs.iso

debug:
	qemu-system-i386 -cdrom kfs.iso -S -s
	gdb -x gdb.cfg kernel.elf

clean:
	rm -f boot.o kernel.elf kfs.iso iso/boot/kernel.elf

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

