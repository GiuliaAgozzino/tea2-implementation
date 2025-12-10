#include <stdio.h>
#include <stdint.h>

uint8_t truth_table_f1[8][16] ={
      {1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1},
      {0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0},
      {1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1},
      {0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0 },
      {0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0},
      {0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
      {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
      {1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0} 
};

uint8_t truth_table_f2[8][16] ={
      {1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0 },
      {0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1 },
      {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
      {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
      {0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 },
      {1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
      {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1},
      {0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1 }
};

uint8_t f1(uint8_t nibble[8]){
        uint8_t result = 0;
        for(int i=0; i<8; i++){
            int row = i;
            int col = nibble[i];
            result = (result << 1) | truth_table_f1[row][col];
        };
            return result;
};
uint8_t f1(uint8_t nibble[8]){
        uint8_t result = 0;
        for(int i=0; i<8; i++){
            int row = i;
            int col = nibble[i];
            result = (result << 1) | truth_table_f1[row][col];
        };
            return result;
};
uint8_t f1(uint8_t nibble[8]){
        uint8_t result = 0;
        for(int i=0; i<8; i++){
            int row = i;
            int col = nibble[i];
            result = (result << 1) | truth_table_f1[row][col];
        };
            return result;
};
uint8_t f1(uint8_t nibble[8]){
        uint8_t result = 0;
        for(int i=0; i<8; i++){
            int row = i;
            int col = nibble[i];
            result = (result << 1) | truth_table_f2[row][col];
        };
            return result;
};

void stampaByte(uint8_t n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}
void main(){
    uint8_t input[8] = {0b1111, 0b0011, 0b1010, 0b1011, 0b0111, 0b0101, 0b0110, 0b0001};
    uint8_t output = f1(input);
      stampaByte(output);
}