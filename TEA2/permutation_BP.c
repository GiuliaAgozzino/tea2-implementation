#include "../utils.h"

int new_order[8] = {4, 8, 5, 7, 2, 1, 3, 6};

uint8_t permutation_BP(uint8_t input) {
    uint8_t output = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t bit = (input >> (8 - new_order[i])) & 0x01;
        output |= bit << (7 - i);
    }

    return output;
}


