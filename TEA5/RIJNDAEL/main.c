#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rijndael.h"

// Stampa un buffer in esadecimale
void print_hex(const char *label, const uint8_t *buf, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", buf[i]);
        if ((i + 1) % 4 == 0) printf(" ");
    }
    printf("\n");
}

int main(void) {

    // Chiave da 192 bit (24 byte) — Rijndael-256 con NK=6
    uint8_t key[24] = {
        0x00, 0x01, 0x02, 0x04,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17
    };

    // Plaintext da 256 bit (32 byte)
    uint8_t plaintext[32] = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff,
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff
    };

    uint8_t ciphertext[32];
    uint8_t decrypted[32];

    printf("=== Rijndael-256 (NB=8, NK=6, NR=14) ===\n\n");

    print_hex("Chiave     ", key, 24);
    print_hex("Plaintext  ", plaintext, 32);

    // Cifratura
    rijndael_encrypt(plaintext, key, ciphertext);
    print_hex("Ciphertext ", ciphertext, 32);

    // Decifratura
    rijndael_decrypt(ciphertext, key, decrypted);
    print_hex("Decrypted  ", decrypted, 32);

    // Verifica
    printf("\n");
    if (memcmp(plaintext, decrypted, 32) == 0) {
        printf("[OK] Decrypt == Plaintext originale\n");
    } else {
        printf("[ERRORE] Il testo decifrato NON corrisponde!\n");
    }

    // Test 2: plaintext tutto zeri
    printf("\n=== Test 2: plaintext tutto zeri ===\n\n");
    uint8_t zero_plain[32] = {0};
    uint8_t zero_cipher[32];
    uint8_t zero_dec[32];

    print_hex("Plaintext  ", zero_plain, 32);
    rijndael_encrypt(zero_plain, key, zero_cipher);
    print_hex("Ciphertext ", zero_cipher, 32);
    rijndael_decrypt(zero_cipher, key, zero_dec);
    print_hex("Decrypted  ", zero_dec, 32);

    printf("\n");
    if (memcmp(zero_plain, zero_dec, 32) == 0) {
        printf("[OK] Decrypt == Plaintext originale\n");
    } else {
        printf("[ERRORE] Il testo decifrato NON corrisponde!\n");
    }

    return 0;
}