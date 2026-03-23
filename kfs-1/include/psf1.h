// psf1.h
#ifndef PSF1_H
#define PSF1_H

#include <stdint.h>

#define PSF1_MAGIC 0x0436

typedef struct {
    uint16_t    magic ;             // must equal PSF1_MAGIC
    uint8_t     mode ;              // 0x02 = 256 glyphs + unicode table
    uint8_t     height ;
} PSF1Header;

#endif
