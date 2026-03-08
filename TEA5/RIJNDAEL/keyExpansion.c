#include <stdint.h>
#include "rijndael_tables.h"

#define NK 6
#define NB 8
#define NR 14
#define WORDS (NB*(NR+1))

const uint8_t Rcon[] = {
    0x00,                          
    0x01, 0x02, 0x04, 0x08, 0x10, 
    0x20, 0x40, 0x80, 0x1B, 0x36, 
    0x6C, 0xD8, 0xAB, 0x4D, 0x9A, 
    0x2F, 0x5E, 0xBC, 0x63        
};
void subWord(uint8_t word[4]){

    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i]];
    }

}

void rotWord(uint8_t word[4]){

    uint8_t temp = word[0];
    for (int i = 0; i < 3; i++) {
        word[i] = word[i+1];
    }
    word[3] = temp;
}


void keyExpansion(const uint8_t key[24], uint8_t W[120][4]){
  
    for(int i=0;i<NK;i++){
        for(int j=0;j<4;j++){
            W[i][j] = key[i*4 + j];
        }
    }

    uint8_t temp[4];

    for(int i=NK;i<120;i++){

        // copia W[i-1] in temp
        for(int j=0;j<4;j++){
            temp[j] = W[i-1][j];
        }

        if(i % NK == 0){

            rotWord(temp);
            subWord(temp);

            temp[0] ^= Rcon[i/NK];

        }

        for(int j=0;j<4;j++){
            W[i][j] = W[i-NK][j] ^ temp[j];
        }
    }
}