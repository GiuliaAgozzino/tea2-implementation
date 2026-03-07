
void addRoundKey(uint8_t state[4][8], uint8_t round_key[4][8]){

    for(int r=0; r<4; r++) {
        for(int c=0; c<8; c++) {
            state[r][c] ^= round_key[r][c];
        }
    }
}