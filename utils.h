#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef __uint128_t uint80_t;

typedef struct {
    uint8_t  ts;    // timeslot
    uint8_t  fn;    // frame
    uint8_t  mn;    // multiframe
    uint16_t hnf;   // hyperframe
    uint8_t  dir;   // uplink=1, downlink=0
} ivStruct;

// dichiarazioni funzioni
uint80_t tb5(uint80_t ck, uint16_t cn, uint8_t cc, uint16_t la);
void iv_loading(uint32_t iv, uint8_t R[8]);
void eck_loading(uint80_t eck, uint8_t K[10]);

// Funzioni TEA2
void tea2(uint32_t num_byte_ks, uint8_t R[8], uint8_t K[10], uint8_t ks[]);

// Funzioni di supporto
uint8_t permutation_P(uint8_t input);
uint8_t permutation_BP(uint8_t input);
void    expander_E(uint8_t first_byte, uint8_t second_byte, uint8_t result[8]);
uint8_t f1(uint8_t nibble[8]);
uint8_t f2(uint8_t nibble[8]);

#endif /* UTILS_H */