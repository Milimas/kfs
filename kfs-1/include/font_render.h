// font_render.h
#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include <stdint.h>
#include "psf1.h"

#define FONT_WIDTH 8

void putchar(uint32_t *fb, uint32_t pitch,
               PSF1Header *font,
               char c,
               uint32_t x, uint32_t y,
               uint32_t fg, uint32_t bg) {

    uint8_t *glyph = (uint8_t *)font
                     + 4 + ((uint8_t)c * font->height) ;

    uint32_t bytes_per_row = 1 ;

    for (uint32_t row = 0; row < font->height; row++) {
        for (uint32_t col = 0; col < FONT_WIDTH; col++) {
            // which byte and which bit within that byte
            uint8_t byte = glyph[row * bytes_per_row + (col / 8)];
            uint8_t bit  = byte & (0x80 >> (col % 8));

            fb[(y + row) * (pitch / 4) + (x + col)] = bit ? fg : bg;
        }
    }
}

void puts(uint32_t *fb, uint32_t pitch,
                 PSF1Header *font,
                 const char *str,
                 uint32_t x, uint32_t y,
                 uint32_t fg, uint32_t bg) {
    uint32_t start_x = x ;
    while (*str) {
        if (*str == '\n') {
            y += font->height ;
            x = start_x ;
            ++str ;
            continue ;
        }
        putchar(fb, pitch, font, *str, x, y, fg, bg);
        x += FONT_WIDTH;  // advance by one glyph width
        ++str ;
    }
    y += font->height ;
    x = start_x ;
}

#endif
