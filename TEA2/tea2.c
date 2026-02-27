#include "../utils.h"


void tea2(uint32_t num_byte_ks, uint8_t R[8], uint8_t K[10], uint8_t ks[] ) {

    uint8_t skip_rounds = 51;

    for (int i = 0; i < num_byte_ks; i++) {

        for (int j = 0; j < skip_rounds; j++) {
            
         uint8_t BP_R5 = permutation_BP(R[5]);

         uint8_t E_R4R3[8];
         expander_E(R[4], R[3], E_R4R3);

         uint8_t F_R4R3 = f2(E_R4R3);

         uint8_t E_R1R0[8];
         expander_E(R[1], R[0], E_R1R0);

         uint8_t F_R1R0 = f2(E_R1R0);

         uint8_t new_R3 = F_R4R3 ^ R[2]
         uint8_t new_R0 = BP_R5 ^ R[2] ^ F_R1R0






        }


    }

}