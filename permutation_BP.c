#include <stdio.h>
#include <inttypes.h>

int new_order[8] = {4, 8, 5, 7, 2, 1, 3, 6};

uint8_t permutation_BP(uint8_t input) {
    uint8_t output = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t bit = (input >> (8 - new_order[i])) & 0x01;
        output |= bit << (7 - i);
    }

    return output;
}

void stampaByte(uint8_t n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int main() {
    uint8_t input = 0b10011011;
    uint8_t output = permutation_BP(input);

    printf("Output: ");
    stampaByte(output);

    return 0;
}
