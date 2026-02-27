#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

typedef __uint128_t uint80_t;

typedef struct {
    uint8_t  ts;    // timeslot
    uint8_t  fn;    // frame
    uint8_t  mn;    // multiframe
    uint16_t hnf;   // hyperframe
    uint8_t  dir;   // uplink=1, downlink=0
} ivStruct;

#endif /* UTILS_H */