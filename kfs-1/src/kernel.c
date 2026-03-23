// kernel main function
#include <stdint.h>
#include "psf1.h"
#include "font_render.h"

void kmain(uint32_t magic, uint32_t *mb_info) {
    if (magic != 0x36d76289) return;

    uint8_t *tag = (uint8_t *)mb_info + 8;

    uint32_t *fb      = 0;
    uint32_t  pitch   = 0;
    uint32_t  width   = 0;
    uint32_t  height  = 0;
    PSF1Header *font  = 0;

    while (1) {
        uint32_t type = *(uint32_t *)tag;
        uint32_t size = *(uint32_t *)(tag + 4);

        if (type == 0) break;

        if (type == 8) { // framebuffer
            uint64_t fb_addr = *(uint64_t *)(tag + 8);
            pitch  = *(uint32_t *)(tag + 16);
            width  = *(uint32_t *)(tag + 20);
            height = *(uint32_t *)(tag + 24);
            fb = (uint32_t *)(uint32_t)fb_addr;
        }

        if (type == 3) { // module = our font
            uint32_t mod_start = *(uint32_t *)(tag + 8);
            font = (PSF1Header *)(uint32_t)mod_start;
        }

        tag += (size + 7) & ~7;
    }

    if (!fb || !font) return;
    if (font->magic != PSF1_MAGIC) return; // sanity check

    // clear to black
    for (uint32_t y = 0; y < height; y++)
        for (uint32_t x = 0; x < width; x++)
            fb[y * (pitch / 4) + x] = 0x00000000;

    // draw some text
    uint32_t x  = 100 ;
    uint32_t y  = 100 ;
    uint32_t lh = font->height ;

    puts(fb, pitch, font, "SYSTEM: 1337",            x, y,        0x00FFFFFF, 0x00000000) ; y += lh ;
    puts(fb, pitch, font, "NETWORK: 42",             x, y,        0x00FFFFFF, 0x00000000) ; y += lh ;
    puts(fb, pitch, font, "STATUS: LEET",            x, y,        0x00FFFFFF, 0x00000000) ; y += lh * 4 ;
    puts(fb, pitch, font, "WAKE UP, NEO...",         x, y,        0x00FFFFFF, 0x00000000) ; y += lh ;
    puts(fb, pitch, font, "THE MATRIX HAS YOU...",   x, y,        0x00FFFFFF, 0x00000000) ; y += lh ;
    puts(fb, pitch, font, "FOLLOW THE WHITE RABBIT.", x, y,       0x00FFFFFF, 0x00000000) ; y += lh ;
    puts(fb, pitch, font, "KNOCK, KNOCK, NEO.",      x, y,        0x00FFFFFF, 0x00000000) ;
                
}
