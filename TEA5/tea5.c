#include "../utils.h"
#include "RIJNDAEL/rijndael.h" 
#include <string.h>
#include <stdint.h>




void tea5(uint32_t num_byte_ks,  uint8_t ck[24],  uint80_t iv, uint8_t ks[]){

    uint8_t ivx[24];
    iv_expander(iv, ivx);

    uint8_t ckm[24];
    uint8_t ivm[24];

    f(ck, ivx, ckm, ivm);

    int num_blocks = ((num_byte_ks * 8) + 255) / 256;  // ceil(length / 256 bit)
    int length = num_byte_ks * 8;

    for (int j = 0; j < num_blocks; j++) {

        uint8_t plain[32];

        memcpy(plain, ivm, 24);

        plain[24] = 'T';
        plain[25] = 'E';
        plain[26] = 'A';
        plain[27] = '5';

        plain[28] = (j >> 24) & 0xFF;
        plain[29] = (j >> 16) & 0xFF;
        plain[30] = (j >> 8) & 0xFF;
        plain[31] = j & 0xFF;

        uint8_t cipher[32];
        rijndael_encrypt(plain, ckm, cipher);

        int bits_done   = j * 256;
        int bits_remain = length - bits_done;

        int bytes_copy = (bits_remain >= 256) ? 32 : (bits_remain + 7) / 8;

       int offset = j * 32;
       memcpy(ks + offset, cipher, bytes_copy);
    }

    /* Maschera i bit inutili dell’ultimo byte */
    if(length % 8 != 0){

        int last_byte_index = (length - 1) / 8;
        int unused_bits = 8 - (length % 8);

        ks[last_byte_index] &= (0xFF << unused_bits);
    }
}