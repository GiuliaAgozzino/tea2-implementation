#include <stdio.h>
#include <inttypes.h>
typedef struct {
    uint64_t high;  // bit 79..16
    uint16_t low;   // bit 15..0
} uint80_t;

void split_key_80bit(uint80_t key, uint8_t K[10]) {
 
    for (int i = 0; i < 8; i++) {
        K[i] = (key.high >> (56 - 8*i)) & 0xFF;
    }

    K[8] = (key.low >> 8) & 0xFF;
    K[9] = key.low & 0xFF;
}
