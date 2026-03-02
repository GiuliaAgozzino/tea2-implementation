#include "../utils.h"

uint80_t tb5(uint80_t ck, uint16_t cn, uint8_t cc, uint16_t la) {
    uint80_t s;

s =  ((uint80_t)cn)
   | ((uint80_t)cc << 12)
   | ((uint80_t)cn << 18)
   | ((uint80_t)cc << 30)
   | ((uint80_t)cn << 36)
   | ((uint80_t)cc << 48)
   | ((uint80_t)cn << 54)
   | ((uint80_t)la << 66);      

    return ck ^ s;
}