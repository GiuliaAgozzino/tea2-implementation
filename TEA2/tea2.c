#include "../utils.h"


void tea2(uint32_t num_byte_ks, uint8_t R[8], uint8_t K[10], uint8_t ks[]) {

    uint32_t skip_rounds = 51;  

    for (int i = 0; i < num_byte_ks; i++) {

        for (int j = 0; j < skip_rounds; j++) {

        
            uint8_t new_K0 = permutation_P(K[9] ^ K[2]);

            
            uint8_t BP_R5 = permutation_BP(R[5]);

            uint8_t E_R1R0[8];
            expander_E(R[1], R[0], E_R1R0);
            uint8_t F1_R1R0 = f1(E_R1R0);  

            uint8_t E_R4R3[8];
            expander_E(R[4], R[3], E_R4R3);
            uint8_t F2_R4R3 = f2(E_R4R3);  

            uint8_t new_R0 = R[7] ^ BP_R5 ^ R[2] ^ F1_R1R0 ^ new_K0;
            uint8_t new_R3 = R[2] ^ F2_R4R3;

            // shift registro chiave
           memmove(K + 1, K, 9);
           K[0] = new_K0;

            // shift registro output
            memmove(R + 1, R, 7);
            R[0] = new_R0;
            R[3] = new_R3;
        }

        ks[i] = R[7];
        skip_rounds = 19;
    }
}