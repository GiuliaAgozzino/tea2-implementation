#include <stdio.h>
#include <stdint.h>

void iv_expansion(uint8_t iv[4], uint8_t iv_loaded[4]) {
    // esempio: copia e modifica
    for (int i = 0; i < 4; i++) {
        out[i] = iv[i] ^ 0xAA;   // trasformazione a caso
    }
}