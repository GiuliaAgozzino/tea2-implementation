#include "rijndael_tables.h"
#include <stdint.h>

void subByte(uint8_t state[4][8]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            state[i][j] = sbox[state[i][j]];      
        }
    }
}



void invSubByte(uint8_t state[4][8]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            state[i][j] = inv_sbox[state[i][j]];  
        }
    }
}