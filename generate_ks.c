#include "utils.h"

// formIVTEA2 — IV da 29 bit per TEA set A
 
uint32_t formIVTEA2(IvTEA2 iv)
{
    return  ((uint32_t)(iv.ts - 1U)          )        /* bit 0-1  */
          | ((uint32_t) iv.fn          << 2U )        /* bit 2-6  */
          | ((uint32_t) iv.mn          << 7U )        /* bit 7-12 */
          | ((uint32_t)(iv.hnf & 0x7FFFu) << 13U)    /* bit 13-27 */
          | ((uint32_t)(iv.dir & 0x1u)    << 28U);   /* bit 28   */
}

// formIVTEA5 — IV da 80 bit per TEA set B

uint80_t formIVTEA5(IvTEA5 iv)
{
    return  (uint80_t)(iv.ts - 1U)                       /* bit 0-1   */
          | ((uint80_t) iv.fn              << 2U )       /* bit 2-6   */
          | ((uint80_t) iv.mn              << 7U )       /* bit 7-12  */
          | ((uint80_t)(iv.hnf & 0xFFFFu)  << 13U)      /* bit 13-28 */
          | ((uint80_t)(iv.dir  & 0x1u)    << 29U)      /* bit 29    */
          | ((uint80_t)(iv.subs & 0x1u)    << 30U)      /* bit 30    */
          | ((uint80_t)(iv.cn   & 0x0FFFu) << 31U)      /* bit 31-42 */
          | ((uint80_t)(iv.la   & 0x3FFFu) << 43U)      /* bit 43-56 */
          | ((uint80_t)(iv.cc   & 0x3Fu)   << 57U)      /* bit 57-62 */
          | ((uint80_t)(iv.pdu  & 0xFFu)   << 63U);     /* bit 63-70 */
          /* bit 71-79 = 0 implicitamente */
}

// converto una stringa esadecimale in uint80_t
 
uint80_t hex_to_uint80(const char *s)
{
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    uint80_t result = 0;
    while (*s) {
        uint8_t nibble;
        if      (*s >= '0' && *s <= '9') nibble = (uint8_t)(*s - '0');
        else if (*s >= 'a' && *s <= 'f') nibble = (uint8_t)(*s - 'a' + 10);
        else if (*s >= 'A' && *s <= 'F') nibble = (uint8_t)(*s - 'A' + 10);
        else {
            fprintf(stderr, "Errore: carattere non valido nella chiave: '%c'\n", *s);
            exit(EXIT_FAILURE);
        }
        result = (result << 4U) | nibble;
        s++;
    }
    return result;
}

// converte una stringa esadecimale in array di byte
 
void hex_to_bytes(const char *hex, uint8_t *out, int len)
{
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
        hex += 2;

    for (int i = 0; i < len; i++) {
        unsigned int byte;
        if (sscanf(hex + 2 * i, "%2x", &byte) != 1) {
            fprintf(stderr, "Errore: stringa esadecimale non valida\n");
            exit(EXIT_FAILURE);
        }
        out[i] = (uint8_t)byte;
    }
}

// TEA2
void generateKeyStreamTEA2(uint8_t ks[], const char *ck_hex, uint32_t num_bytes,
                            int ts, int fn, int mn, int hnf,
                            int dir, int cc, int cn, int la)
{
    uint80_t ck  = hex_to_uint80(ck_hex);

    IvTEA2 ivc = {
        .ts  = (uint8_t)  ts,
        .fn  = (uint8_t)  fn,
        .mn  = (uint8_t)  mn,
        .hnf = (uint16_t) hnf,
        .dir = (uint8_t)  dir,
    };

    uint80_t eck = tb5(ck, (uint16_t)cn, (uint8_t)cc, (uint16_t)la);
    uint32_t iv  = formIVTEA2(ivc);

    uint8_t R[8];
    uint8_t K[10];
    iv_loading(iv, R);
    eck_loading(eck, K);

    tea2(num_bytes, R, K, ks);
}

// TEA5
void generateKeyStreamTEA5(uint8_t ks[], const char *ck_hex, uint32_t num_bytes,
                            int ts, int fn, int mn, int hnf,
                            int dir, int subs, int cn, int la,
                            int cc, int pdu)
{
    uint8_t ck[24];
    hex_to_bytes(ck_hex, ck, 24);

    IvTEA5 ivc = {
        .ts   = (uint8_t)  ts,
        .fn   = (uint8_t)  fn,
        .mn   = (uint8_t)  mn,
        .hnf  = (uint16_t) hnf,
        .dir  = (uint8_t)  dir,
        .subs = (uint8_t)  subs,
        .cn   = (uint16_t) cn,
        .la   = (uint16_t) la,
        .cc   = (uint8_t)  cc,
        .pdu  = (uint8_t)  pdu,
    };

    uint80_t iv = formIVTEA5(ivc);

    uint8_t ivx[24];
    iv_expander(iv, ivx);

    uint8_t ckm[24];
    uint8_t ivm[24];
    f(ck, ivx, ckm, ivm);

    
    tea5(num_bytes, ckm, ivm, ks);
}