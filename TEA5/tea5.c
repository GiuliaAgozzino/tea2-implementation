#include "../utils.h"
#include "rijndael-api-fst.h"
#include <string.h>
#include <stdint.h>

void tea5(uint32_t num_byte_ks, uint8_t ivm[24], uint8_t ckm[24], uint8_t ks[]){
    
}


void tea5_keystream(const uint8_t *CKM, const uint8_t *IVM,
                    uint8_t *kss, int length)
{
    keyInstance    ki;
    cipherInstance ci;

    /* makeKey vuole la chiave come stringa hex ASCII */
    char key_hex[49];
    for (int i = 0; i < 24; i++)
        sprintf(&key_hex[2*i], "%02X", CKM[i]);
    key_hex[48] = '\0';

    makeKey(&ki, DIR_ENCRYPT, 192, key_hex);
    cipherInit(&ci, MODE_ECB, NULL);

    int num_blocks = (length + 255) / 256;  /* ceil(length/256) */

    for (int j = 0; j < num_blocks; j++) {

        /* Costruisci il plaintext: IVM || 'T','E','A','5' || j (4 byte BE) */
        uint8_t plain[32];

        memcpy(plain, IVM, 24);          /* byte 0..23  = IVM        */
        plain[24] = 84;                  /* 'T'                      */
        plain[25] = 69;                  /* 'E'                      */
        plain[26] = 65;                  /* 'A'                      */
        plain[27] = 53;                  /* '5'                      */
        plain[28] = (j >> 24) & 0xFF;   /* counter big-endian       */
        plain[29] = (j >> 16) & 0xFF;
        plain[30] = (j >>  8) & 0xFF;
        plain[31] = (j      ) & 0xFF;

        /* Rijndael vuole i dati come bit-array — blockEncrypt li
           legge e scrive in-place, 256 bit = 32 byte             */
        uint8_t cipher[32];
        blockEncrypt(&ci, &ki, (BYTE*)plain, 256, (BYTE*)cipher);

        /* Copia i byte del blocco nel buffer kss */
        int bits_done   = j * 256;
        int bits_remain = length - bits_done;       /* bit ancora da produrre */
        int bytes_copy  = (bits_remain >= 256) ? 32 /* blocco intero */
                                                : (bits_remain + 7) / 8;
        memcpy(kss + j * 32, cipher, bytes_copy);
    }

    /* Se length non è multiplo di 8, azzera i bit in eccesso
       nell'ultimo byte (i bit meno significativi, §5.4)        */
    if (length % 8 != 0) {
        int last = length / 8;
        kss[last] &= (0xFF << (8 - (length % 8))) & 0xFF;
    }
}