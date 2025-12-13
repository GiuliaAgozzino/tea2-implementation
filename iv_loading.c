#include <stdio.h>
#include <stdint.h>

const uint32_t mask = 0x5A6E3278;

void iv_loading(uint32_t iv29, uint8_t R[8]) {

    uint32_t iv32 = iv29 & 0x1FFFFFFF;

    uint8_t F1 = (iv32 >> 24) & 0xFF;  
    uint8_t F2 = (iv32 >> 16) & 0xFF;
    uint8_t F3 = (iv32 >>  8) & 0xFF;
    uint8_t F4 =  iv32        & 0xFF;  

    R[7] = F1 ^ ((mask >> 24) & 0xFF); // 5A
    R[6] = F1;
    R[5] = F2;
    R[4] = F3;
    R[3] = F4;
    R[2] = F2 ^ ((mask >> 16) & 0xFF); // 6E
    R[1] = F3 ^ ((mask >> 8)  & 0xFF); // 32
    R[0] = F4 ^ ( mask        & 0xFF); // 78
}