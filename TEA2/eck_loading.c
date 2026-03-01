#include "../utils.h"

void eck_loading(uint80_t key, uint8_t K[10]) {
    for (int i = 0; i < 10; i++) {
        K[9-i] = (key >> (72 - 8*i)) & 0xFF;
    }
}