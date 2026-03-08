#include "rijndael_tables.h"
#include <stdint.h>

void mixColumns(uint8_t state[4][8]) {

    for(int c=0; c<8; c++) {

        uint8_t s0 = state[0][c];
        uint8_t s1 = state[1][c];
        uint8_t s2 = state[2][c];
        uint8_t s3 = state[3][c];

        state[0][c] =
            mul2[s0] ^ mul3[s1] ^ s2 ^ s3;

        state[1][c] =
            s0 ^ mul2[s1] ^ mul3[s2] ^ s3;

        state[2][c] =
            s0 ^ s1 ^ mul2[s2] ^ mul3[s3];

        state[3][c] =
            mul3[s0] ^ s1 ^ s2 ^ mul2[s3];
    }
}

void invMixColumns(uint8_t state[4][8]) {

    for(int c=0; c<8; c++) {

        uint8_t s0 = state[0][c];
        uint8_t s1 = state[1][c];
        uint8_t s2 = state[2][c];
        uint8_t s3 = state[3][c];

        state[0][c] =
            mul14[s0] ^ mul11[s1] ^ mul13[s2] ^ mul9[s3];

        state[1][c] =
            mul9[s0] ^ mul14[s1] ^ mul11[s2] ^ mul13[s3];

        state[2][c] =
            mul13[s0] ^ mul9[s1] ^ mul14[s2] ^ mul11[s3];

        state[3][c] =
            mul11[s0] ^ mul13[s1] ^ mul9[s2] ^ mul14[s3];
    }
}
