# KFS-1 — Booting

The first milestone of the KFS project. Implements a Multiboot2-compliant kernel that boots via GRUB, initializes a stack, and writes to VGA text mode.

## Objectives

- Implement the Multiboot2 header specification
- Boot a 32-bit protected mode kernel using GRUB
- Set up a basic stack in BSS
- Write to VGA text buffer at `0xB8000`
- Clear the screen and print a boot message

## Build

```bash
make          # Build kernel.elf
make iso      # Build bootable ISO
make run      # Build and run in QEMU
make debug    # Build and run with GDB server (-S -s)
make clean    # Remove build artifacts
make help     # Show available targets
```

## Debugging

In a separate terminal after `make debug`:

```bash
make debug    # in terminal 1 (starts QEMU paused)
gdb -x gdb.cfg kernel.elf   # in terminal 2
```

`gdb.cfg` connects to QEMU's GDB server and sets initial breakpoints.

## Project Layout

```
kfs-1/
├── boot.asm       # Multiboot2 header, entry point, VGA routines
├── linker.ld      # Linker script — memory layout at 0x100000
├── gdb.cfg        # GDB startup commands
├── Makefile        # Build system
└── iso/
    └── boot/
        └── grub/
            └── grub.cfg    # GRUB menu configuration
```

## Technical Details

- **Architecture**: i386 (32-bit protected mode)
- **Boot protocol**: Multiboot2
- **Load address**: `0x00100000` (1 MB)
- **VGA text mode**: 80×25, buffer at `0xB8000`
- **Stack**: 16 KB, allocated in BSS
