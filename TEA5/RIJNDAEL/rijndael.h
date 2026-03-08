#ifndef RIJNDAEL_H
#define RIJNDAEL_H

#include <stdint.h>

// Operazioni base
void addRoundKey(uint8_t state[4][8], uint8_t round_key[4][8]);
void subByte(uint8_t state[4][8]);
void invSubByte(uint8_t state[4][8]);
void shiftRows(uint8_t state[4][8]);
void invShiftRows(uint8_t state[4][8]);
void mixColumns(uint8_t state[4][8]);
void invMixColumns(uint8_t state[4][8]);

// Key expansion
void keyExpansion(const uint8_t key[24], uint8_t W[120][4]);

// Encrypt / Decrypt
void rijndael_encrypt(const uint8_t plaintext[32], const uint8_t key[24], uint8_t ciphertext[32]);
void rijndael_decrypt(const uint8_t ciphertext[32], const uint8_t key[24], uint8_t plaintext[32]);

#endif /* RIJNDAEL_H */