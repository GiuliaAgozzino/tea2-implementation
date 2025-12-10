#include <stdio.h>
#include <stdint.h>

int first_two_bits[][2] = {
    {1, 2}, {2, 3}, {3, 4}, {4, 5},
    {5, 6}, {6, 7}, {7, 8}, {8, 1}
};

int second_two_bits[][2] = {
    {7, 8}, {8, 1}, {1, 2}, {2, 3},
    {3, 4}, {4, 5}, {5, 6}, {6, 7}
};

void stampaByte(uint8_t n);

void expander_E(uint8_t first_byte, uint8_t second_byte, uint8_t result[8]) {
    for (int i = 0; i < 8; i++) {
        uint8_t b1 = (((first_byte >> (8 - first_two_bits[i][0])) & 1) << 1) |
                     ((first_byte >> (8 - first_two_bits[i][1])) & 1);

        uint8_t b2 = (((second_byte >> (8 - second_two_bits[i][0])) & 1) << 1) |
                     ((second_byte >> (8 - second_two_bits[i][1])) & 1);

        result[i] = (b1 << 2) | b2;
    }
}

void stampaByte(uint8_t n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int main() {
    uint8_t first_byte = 0b11001100;
    uint8_t second_byte = 0b10100110;
    uint8_t result[8];

    expander_E(first_byte, second_byte, result);

    int lunghezza = sizeof(result) / sizeof(result[0]);
    for (int i = 0; i < lunghezza; i++) {
        stampaByte(result[i]);
    }

    return 0;
}
