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

#endif /* UTILS_H */