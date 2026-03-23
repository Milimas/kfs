# Linker

how to check segments permissions

```bash
readelf -l kernel.elf

# More detail per 
readelf -s kernel.elf

# Or
objdump -h kernel.elf
```
to verify if the kernel image is compatible with GRUB multiboot2 specification for i386 you can use:
```bash
grub-file --is-x86-multiboot2 kernel.elf
```
# Font Pipeline

## Overview

```
FontForge (font/src/kfs.sfd)
    └─► Export BDF  →  font/bdf/kfs-16.bdf
            └─► bdf2psf  →  font/psf/kfs.psf  →  GRUB module2  →  kernel
```

## FontForge → BDF

File → Generate Fonts → format: BDF → resolution: 75 dpi
Output: `font/bdf/kfs-16.bdf`

## BDF → PSF1

```bash
bdf2psf --fb font/bdf/kfs-16.bdf \
    /usr/share/bdf2psf/standard.equivalents \
    /usr/share/bdf2psf/ascii.set \
    256 \
    font/psf/kfs.psf
```

- `--fb` removes the 8px width and 256/512 glyph constraints (framebuffer mode)
- `standard.equivalents` maps Unicode synonyms to shared glyphs
- `ascii.set` ensures printable ASCII glyphs are packed in ASCII order
- `256` = number of glyph slots

Without `ascii.set`, glyph ordering is arbitrary and characters render as garbage.

## PSF format

`bdf2psf` always outputs **PSF1** regardless of `--fb`.

PSF1 header (4 bytes):
```
offset  size  field   value
0       2     magic   0x0436
2       1     mode    0x02 = 256 glyphs + unicode table
3       1     height  e.g. 16
```
Width is always 8 — not stored in the header.
Glyph data starts at byte 4. Each glyph = `height` bytes (1 byte per row, MSB = leftmost pixel).

PSF2 magic for reference: `72 b5 4a 86`

## Glyph index mapping

With `ascii.set`, bdf2psf puts space (U+0020) at glyph index 0.
So the glyph index for a character = `(uint8_t)c - 0x20`.

## Verify PSF magic

```bash
xxd -l 4 font/psf/kfs.psf
# PSF1: 36 04 ...
# PSF2: 72 b5 4a 86
```

## Inspect a glyph (e.g. 'H' = index 40 = 72-32, offset = 4 + 40*16 = 644)

```bash
xxd -s 644 -l 16 font/psf/kfs.psf
# each byte = one row, ff = full row, 00 = empty, c0 = ##......
```

## Dump unicode table

```bash
psfxtable -i font/psf/kfs.psf
```

## GRUB config

```
set gfxmode=1024x768x32
menuentry "name" {
    multiboot2  /boot/kernel.elf
    module2     /boot/font.psf       # loaded as Multiboot2 module, type tag 3
    boot
}
```

`set gfxpayload=keep` is not needed when `set gfxmode` is set outside the menuentry.

## Kernel font loading

Multiboot2 tag type 3 = module. Fields:
```
+0  uint32  type = 3
+4  uint32  size
+8  uint32  mod_start   ← physical address of the PSF file in memory
+12 uint32  mod_end
+16 char[]  string (cmdline, null-terminated)
```

Cast `mod_start` to `PSF1Header*` and validate magic before use.

## Framebuffer tag

Multiboot2 tag type 8 = framebuffer. Fields:
```
+0   uint32  type = 8
+4   uint32  size
+8   uint64  fb_addr    ← physical address of framebuffer
+16  uint32  pitch      ← bytes per row
+20  uint32  width      ← pixels per row
+24  uint32  height     ← rows
+28  uint8   bpp        ← bits per pixel (32 for 32-bit mode)
```

Pixel address: `fb[y * (pitch/4) + x]` for 32bpp (pitch/4 converts bytes→uint32 units).

## old psf files (replaced by custom font)
```bash
ls /usr/share/consolefonts/*.psf.gz
gunzip -k Lat2-Terminus16.psf.gz
```

# i386

x86 General-Purpose Registers (i386 Architecture Style)
===========================================================================

```
31            24 23            16 15             8 7                0  (bit position)
+----------------+----------------+----------------+----------------+
|                          EAX (32 bits)                            |
+----------------+----------------+----------------+----------------+
                                  |            AX (16-bit)          |
                                  +----------------+----------------+
                                  |   AH (8-bit)   |   AL (8-bit)   | 
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         EBX (32 bits)                             |
+----------------+----------------+----------------+----------------+
                                  |            BX (16-bit)          |
                                  +----------------+----------------+
                                  |   BH (8-bit)   |   BL (8-bit)   | 
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         ECX (32 bits)                             |
+----------------+----------------+----------------+----------------+
                                  |            CX (16-bit)          |
                                  +----------------+----------------+
                                  |   CH (8-bit)   |   CL (8-bit)   | 
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         EDX (32 bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             DX (16-bit)         |
                                  +----------------+----------------+
                                  |   DH (8-bit)   |   DL (8-bit)   |
                                  +----------------+----------------+
```

Index and Pointer Registers
===========================================================================

```
31            24 23            16 15             8 7                0  (bit position)
+----------------+----------------+----------------+----------------+
|                         ESI (32-bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             SI (16-bits)        |
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         EDI (32-bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             DI (16-bits)        |
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         EBP (32-bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             BP (16-bits)        |
                                  +----------------+----------------+

+----------------+----------------+----------------+----------------+
|                         ESP (32-bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             SP (16-bits)        |
                                  +----------------+----------------+

```

Instruction Pointer
===========================================================================

```
31            24 23            16 15             8 7                0  (bit position)
+----------------+----------------+----------------+----------------+
|                         EIP (32-bits)                             |
+----------------+----------------+----------------+----------------+
                                  |             IP (16-bit)         |
                                  +----------------+----------------+

```

Segment Selectors (16 bits)
===========================================================================

```
15             8 7                0  (bit position)
+----------------+----------------+
|                CS               |  Code Segment
+----------------+----------------+
|                DS               |  Data Segment
+----------------+----------------+
|                ES               |  Extra Segment
+----------------+----------------+
|                SS               |  Stack Segment
+----------------+----------------+
|                FS               |  General Purpose
+----------------+----------------+
|                GS               |  General Purpose
+----------------+----------------+
```

EFLAGS Register (32 bits)
===========================================================================
-
```
+------+ 31 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 
| 0    |           R
+------+ 22 
| ID   |               X                   ID flag                             (ID)
+------+ 21 
| VIP  |               X                   Virtual Interrupt Pending           (VIP)
+------+ 20 
| VIF  |               X                   Virtual Interrupt Flag              (VIF)
+------+ 19 
| AC   |               X                   Alignment Check                     (AC)
+------+ 18 
| VM   |               X                   Virtual-8086 Mode                   (VM)
+------+ 17 
| RF   |               X                   Resume Flag                         (RF)
+------+ 16    
| 0    |           R
+------+ 15 
| NT   |               X                   Nested Task                         (NT)
+------+ 14 
|      |
+ IOPL + 13            X                   I/O Privilege Level                 (IOPL) 
|      |
+------+ 12 
| OF   |                       S           Overflow Flag                       (OF)
+------+ 11 
| DF   |                   C               Direction Flag                      (DF)
+------+ 10 
| IF   |               X                   Interrupt Enable Flag               (IF)
+------+ 9 
| TF   |               X                   Trap Flag                           (TF)
+------+ 8 
| SF   |                       S           Sign Flag                           (SF)
+------+ 7 
| ZF   |                       S           Zero Flag                           (ZF)
+------+ 6 
| 0    |           R
+------+ 5 
| AF   |                       S           Auxiliary Carry Flag                (AF)
+------+ 4 
| 0    |           R
+------+ 3 
| PF   |                       S           Parity Flag                         (PF)
+------+ 2 
| 1    |           R
+------+ 1 
| CF   |                       S           Carry Flag                          (CF)
-------+ 0 


============================================================

    S           Indicates a Status Flag
    C           Indicates a Control Flag
    X           Indicates a System Flag

    R           Reserved bit positions. DO NOT USE.
                Always set to values previously read.

============================================================

```

Control Registers (32 bits, System Level)
===========================================================================
```
CR0 - System Control (PE, MP, EM, TS, ET, PG)
CR1 - Reserved
CR2 - Page Fault Linear Address
CR3 - Page Directory Base Register
CR4 - Extensions (PSE, PAE, PGE, etc.)
```
