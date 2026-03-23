// psf2.h
#ifndef PSF2_H
#define PSF2_H

#include <stdint.h>

#define PSF2_MAGIC 0x864AB572

typedef struct {
    uint32_t magic;         // must equal PSF2_MAGIC
    uint32_t version;       // 0
    uint32_t header_size;   // offset of bitmap data, usually 32
    uint32_t flags;         // 0 if no unicode table
    uint32_t num_glyphs;    // number of glyphs
    uint32_t bytes_per_glyph;
    uint32_t height;        // in pixels
    uint32_t width;         // in pixels
} PSF2Header;

#endif
