#include "rijndael.h"
#include <stdint.h>

#define NR 14
#define NB 8

// Converte array lineare in state[4][8]
static void bytesToState(const uint8_t in[32], uint8_t state[4][8]) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 8; c++)
            state[r][c] = in[r + 4*c];
}

// Converte state[4][8] in array lineare
static void stateToBytes(const uint8_t state[4][8], uint8_t out[32]) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 8; c++)
            out[r + 4*c] = state[r][c];
}

// Estrae la round key dal W espanso
static void getRoundKey(uint8_t W[120][4], int round, uint8_t rk[4][8]) {
    for (int c = 0; c < NB; c++)
        for (int r = 0; r < 4; r++)
            rk[r][c] = W[round * NB + c][r];
}

void rijndael_encrypt(const uint8_t plaintext[32], const uint8_t key[24], uint8_t ciphertext[32]) {
    uint8_t state[4][8];
    uint8_t W[120][4];
    uint8_t rk[4][8];

    keyExpansion(key, W);
    bytesToState(plaintext, state);

    // Round 0: solo AddRoundKey
    getRoundKey(W, 0, rk);
    addRoundKey(state, rk);

    // Round 1..NR-1
    for (int round = 1; round < NR; round++) {
        subByte(state);
        shiftRows(state);
        mixColumns(state);
        getRoundKey(W, round, rk);
        addRoundKey(state, rk);
    }

    // Round finale: senza MixColumns
    subByte(state);
    shiftRows(state);
    getRoundKey(W, NR, rk);
    addRoundKey(state, rk);

    stateToBytes(state, ciphertext);
}

void rijndael_decrypt(const uint8_t ciphertext[32], const uint8_t key[24], uint8_t plaintext[32]) {
    uint8_t state[4][8];
    uint8_t W[120][4];
    uint8_t rk[4][8];

    keyExpansion(key, W);
    bytesToState(ciphertext, state);

    // Round finale inverso
    getRoundKey(W, NR, rk);
    addRoundKey(state, rk);

    // Round NR-1..1
    for (int round = NR - 1; round >= 1; round--) {
        invShiftRows(state);
        invSubByte(state);
        getRoundKey(W, round, rk);
        addRoundKey(state, rk);
        invMixColumns(state);
    }

    // Round 0
    invShiftRows(state);
    invSubByte(state);
    getRoundKey(W, 0, rk);
    addRoundKey(state, rk);

    stateToBytes(state, plaintext);
}