#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <utils.h>

uint32_t formIV(ivStruct *iv){
    
    assert( 1<= iv->ts && iv->ts <= 4); //timeslot deve essere compreso tra 1 e 4
    assert( 1<= iv->fn && iv->fn <= 18); //frame deve essere compreso tra 1 e 18
    assert( 1<= iv->mn && iv->mn <= 60); //multiframe deve essere compreso tra 1 e 60
    assert( 0<= iv->hnf && iv->hnf <= 0xFFFF); //hyperframe compreso tra 1 e 65535
    assert( iv->dir == 0 || iv->dir == 1); //direction deve essere 0 o 1

     return ((iv->ts - 1)
          | (iv->fn << 2)
          | (iv->mn << 7)
          | (iv->hnf << 13)
          | (iv->dir << 28));
}