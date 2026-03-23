# Makefile -- kernel build system

# ── tools ─────────────────────────────────────────────────────────────────────

AS         = nasm
CC         = gcc
LD         = ld
FONTFORGE  = fontforge
BDF2PSF    = bdf2psf

# ── flags ─────────────────────────────────────────────────────────────────────

ASFLAGS  = -f elf32 -g -F dwarf
CFLAGS   = -m32 -fno-builtin -fno-exceptions -fno-stack-protector \
           -nostdlib -nodefaultlibs -g -Iinclude
LDFLAGS  = -m elf_i386

# ── paths ─────────────────────────────────────────────────────────────────────

SRC_DIR   = src
BUILD_DIR = build
ISO_DIR   = iso
KERNEL    = kernel.elf
ISO       = kfs.iso
GDB_CFG   = gdb.cfg

FONT_SRC  = font/src/kfs.sfd
FONT_BDF  = font/bdf/kfs-16.bdf
FONT_BDF_TARGET = font/bdf/kfs.bdf
FONT_PSF  = font/psf/kfs.psf
FONT_EQV  = /usr/share/bdf2psf/standard.equivalents
FONT_SET  = /usr/share/bdf2psf/ascii.set

# ── sources ───────────────────────────────────────────────────────────────────

ASM_SRC  = boot.asm
C_SRCS   = $(wildcard $(SRC_DIR)/*.c)

ASM_OBJ  = $(BUILD_DIR)/boot.o
C_OBJS   = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS     = $(C_OBJS:.o=.d)

OBJS     = $(ASM_OBJ) $(C_OBJS)

# ── targets ───────────────────────────────────────────────────────────────────

.PHONY: all iso run debug clean help font

all: $(KERNEL)

# link
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld $^ -o $@

# assemble
$(BUILD_DIR)/boot.o: $(ASM_SRC) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

# create build dir if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ── font ──────────────────────────────────────────────────────────────────────

$(FONT_BDF): $(FONT_SRC)
	mkdir -p font/bdf
	$(FONTFORGE) -c 'import fontforge; f=fontforge.open("$<"); f.generate("$(FONT_BDF_TARGET)"); f.close()'

$(FONT_PSF): $(FONT_BDF)
	mkdir -p font/psf
	$(BDF2PSF) --fb $< $(FONT_EQV) $(FONT_SET) 256 $@

font: $(FONT_PSF)

# ── iso ───────────────────────────────────────────────────────────────────────

iso: $(KERNEL) $(FONT_PSF)
	cp grub.cfg $(ISO_DIR)/boot/grub/
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp $(FONT_PSF) $(ISO_DIR)/boot/font.psf
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# ── run ───────────────────────────────────────────────────────────────────────

run: iso
	qemu-system-i386 -cdrom $(ISO)

# ── debug ─────────────────────────────────────────────────────────────────────

# launches QEMU suspended and waits for GDB to connect on port 1234
qemu-debug: iso
	qemu-system-i386 -cdrom $(ISO) \
	    -S -s \
	    -display none \
	    -serial stdio

# launches GDB TUI connected to the running QEMU instance
# tui enable   -- turns on the split source/asm view
# layout split -- shows source + disassembly + registers at once
# layout regs  -- adds register panel
debug: $(KERNEL)
	gdb $(KERNEL) \
	    -ex "set architecture i386" \
	    -ex "target remote :1234" \
	    -ex "tui enable" \
	    -ex "layout split" \
	    -ex "layout regs" \
	    -ex "focus cmd"

# ── clean ─────────────────────────────────────────────────────────────────────

clean:
	rm -rf $(BUILD_DIR) $(KERNEL) $(ISO) \
	       $(ISO_DIR)/boot/kernel.elf \
	       $(ISO_DIR)/boot/font.psf \
	       $(ISO_DIR)/boot/grub/grub.cfg \
	       $(FONT_BDF) $(FONT_BDF_TARGET) $(FONT_PSF)

# ── help ──────────────────────────────────────────────────────────────────────

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "  all         - Build $(KERNEL) (default)"
	@echo "  iso         - Build bootable ISO"
	@echo "  run         - Build and run in QEMU"
	@echo "  qemu-debug  - Run QEMU suspended, waiting for GDB on :1234"
	@echo "  debug       - Launch GDB TUI and connect to QEMU"
	@echo "  font        - Build font (SFD -> BDF -> PSF)"
	@echo "  clean       - Remove all build artifacts"
	@echo "  help        - Show this message"
