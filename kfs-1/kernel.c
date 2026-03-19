#include <stdint.h>

int kmain(uint32_t magic, char* mb_info) {
    if (magic != 0x36d76289) {
        return 1 ;
    }
    while (1) { }
} 
