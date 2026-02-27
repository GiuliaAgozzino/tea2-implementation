#include <stdio.h>
#include <utils.h>


uint80_t tb5(uint80_t ck, uint16_t cn, uint8_t cc, uint16_t la) {
    uint80_t s;

    s = ((uint80_t)cn    << 68)
      | ((uint80_t)cc    << 62)
      | ((uint80_t)cn    << 50)
      | ((uint80_t)cc    << 44)
      | ((uint80_t)cn    << 32)
      | ((uint80_t)cc    << 26)
      | ((uint80_t)cn    << 14)
      | ((uint80_t)la);        // LAid occupa i 14 bit meno significativi

    return ck ^ s;
}