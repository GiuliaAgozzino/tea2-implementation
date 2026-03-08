#include <stdint.h>

void shiftRows(uint8_t state[4][8]) {

    int shift[4] = {0, 1, 3, 4};
    uint8_t temp[8];

    for(int i=0; i<4; i++) {

        for(int j=0; j<8; j++) {
            temp[j] = state[i][(j + shift[i]) % 8];
        }

        for(int j=0; j<8; j++) {
            state[i][j] = temp[j];
        }
    }
}

void invShiftRows(uint8_t state[4][8]) {

    int shift[4] = {0, 1, 3, 4};
    uint8_t temp[8];

    for(int i=0; i<4; i++) {

        for(int j=0; j<8; j++) {
            temp[(j + shift[i]) % 8] = state[i][j];
        }

        for(int j=0; j<8; j++) {
            state[i][j] = temp[j];
        }
    }
}