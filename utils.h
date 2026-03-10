#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>


typedef __uint128_t uint80_t;

// strutture IV                                                            


typedef struct {
    uint8_t  ts;   // timeslot     [1-4]       
    uint8_t  fn;   // frame        [1-18]      
    uint8_t  mn;   // multiframe   [1-60]      
    uint16_t hnf;  // hyperframe   [0-32767]   
    uint8_t  dir;  // 0=downlink, 1=uplink     
} IvTEA2;

typedef struct {
    uint8_t  ts;   // timeslot     [1-4]       
    uint8_t  fn;   // frame        [1-18]      
    uint8_t  mn;   // multiframe   [1-60]      
    uint16_t hnf;  // hyperframe   [0-65535]   
    uint8_t  dir;  // 0=downlink, 1=uplink     
    uint8_t  subs; // subslot      [0-1]       
    uint16_t cn;   // carrier number [0-4095]  
    uint16_t la;   // location area  [0-16383] 
    uint8_t  cc;   // colour code    [0-63]    
    uint8_t  pdu;  // PDU counter    [0-255]   
} IvTEA5;

// costruzione IV 
uint32_t formIVTEA2(IvTEA2 iv);
uint80_t formIVTEA5(IvTEA5 iv);

// conversione da esadecimale 
uint80_t hex_to_uint80(const char *s);
void     hex_to_bytes (const char *hex, uint8_t *out, int len);

// derivazione chiave (TEA2)                                              
uint80_t tb5(uint80_t ck, uint16_t cn, uint8_t cc, uint16_t la);
void     iv_loading (uint32_t iv,  uint8_t R[8]);
void     eck_loading(uint80_t eck, uint8_t K[10]);

// generazione key stream                                                
void tea2(uint32_t num_bytes, uint8_t R[8], uint8_t K[10], uint8_t ks[]);
void tea5(uint32_t num_byte_ks, const uint8_t ckm[24], const  uint8_t ivm[24], uint8_t ks[]);

// funzioni interne TEA2                                                 
uint8_t permutation_P (uint8_t input);
uint8_t permutation_BP(uint8_t input);
void    expander_E(uint8_t first, uint8_t second, uint8_t result[8]);
uint8_t f1(uint8_t nibble[8]);
uint8_t f2(uint8_t nibble[8]);

// funzioni interne TEA5   
void iv_expander(uint80_t iv, uint8_t ivx[24]);
void f(const uint8_t ck[24], const uint8_t ivx[24], uint8_t ckm[24], uint8_t ivm[24]);

// generazione key stream (usate da main)                  
void generateKeyStreamTEA2(uint8_t ks[], const char *ck_hex, int num_bytes,
                            int ts, int fn, int mn, int hnf,
                            int dir, int cc, int cn, int la);

void generateKeyStreamTEA5(uint8_t ks[], const char *ck_hex, int num_bytes,
                            int ts, int fn, int mn, int hnf,
                            int dir, int subs, int cn, int la,
                            int cc, int pdu);

#endif /* UTILS_H */