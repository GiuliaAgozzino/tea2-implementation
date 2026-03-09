#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rijndael.h"

//TEST Rijndael-256 (NB=8, NK=6, NR=14)

/*
// Stampa in esadecimale
void print_hex(const char *label, const uint8_t *buf, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", buf[i]);
        if ((i + 1) % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main(void) {

    // Chiave 192 bit (24 byte)
uint8_t key[24] = {
     0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

// Plaintext 256 bit (32 byte)

   uint8_t plaintext[32] = {
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

    uint8_t ciphertext[32];
    uint8_t decrypted[32];

    printf("=== Rijndael-256 (NB=8, NK=6) Test Vector ===\n\n");

    print_hex("Key        ", key, 24);
    print_hex("Plaintext  ", plaintext, 32);

    // cifratura
    rijndael_encrypt(plaintext, key, ciphertext);
    print_hex("Ciphertext ", ciphertext, 32);


    // decifratura
    rijndael_decrypt(ciphertext, key, decrypted);
    print_hex("\nDecrypted  ", decrypted, 32);

    printf("\n");
    if (memcmp(plaintext, decrypted, 32) == 0) {
        printf("[OK] Decrypt == Plaintext originale\n");
    } else {
        printf("[ERRORE] Il testo decifrato NON corrisponde!\n");
    }

    return 0;
}

*/