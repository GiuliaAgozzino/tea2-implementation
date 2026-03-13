#include "utils.h"
#include <time.h>

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

static void print_hex(const uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
        printf("%02X", buf[i]);
}

static uint8_t rand_byte(void)
{
    return (uint8_t)(rand() & 0xFF);
}


static void gen_ck_random(uint8_t *ck, int len)
{
    for (int i = 0; i < len; i++)
        ck[i] = rand_byte();
}


typedef struct {
    const char *nome;
    uint8_t     ck[10];      /* 80 bit */
    int         ts, fn, mn, hnf, dir, cc, cn, la;
    /* stato interno dopo preparazione */
    uint8_t     K[10];
    uint8_t     R[8];
} VettoreTEA2;

typedef struct {
    const char *nome;
    uint8_t     ck[24];      /* 192 bit */
    int         ts, fn, mn, hnf, dir, cc, cn, la, subs, pdu;
    /* stato interno dopo preparazione */
    uint8_t     ckm[24];
    uint8_t     ivm[24];
} VettoreTEA5;


static void calcola_stato_tea2(VettoreTEA2 *v)
{
    /* ricostruisce uint80_t dalla chiave */
    uint80_t ck = 0;
    for (int i = 0; i < 10; i++)
        ck = (ck << 8) | v->ck[i];

    IvTEA2 ivc = {
        .ts  = (uint8_t) v->ts,
        .fn  = (uint8_t) v->fn,
        .mn  = (uint8_t) v->mn,
        .hnf = (uint16_t)v->hnf,
        .dir = (uint8_t) v->dir,
    };

    uint80_t eck = tb5(ck, (uint16_t)v->cn,
                           (uint8_t) v->cc,
                           (uint16_t)v->la);
    uint32_t iv  = formIVTEA2(ivc);

    iv_loading(iv,  v->R);
    eck_loading(eck, v->K);
}


static void calcola_stato_tea5(VettoreTEA5 *v)
{
    IvTEA5 ivc = {
        .ts   = (uint8_t) v->ts,
        .fn   = (uint8_t) v->fn,
        .mn   = (uint8_t) v->mn,
        .hnf  = (uint16_t)v->hnf,
        .dir  = (uint8_t) v->dir,
        .subs = (uint8_t) v->subs,
        .cn   = (uint16_t)v->cn,
        .la   = (uint16_t)v->la,
        .cc   = (uint8_t) v->cc,
        .pdu  = (uint8_t) v->pdu,
    };

    uint80_t iv = formIVTEA5(ivc);

    uint8_t ivx[24];
    iv_expander(iv, ivx);
    f(v->ck, ivx, v->ckm, v->ivm);
}


static void stampa_vettore_tea2(const VettoreTEA2 *v)
{
    printf("  [%s]\n", v->nome);

    printf("  CK  = ");  print_hex(v->ck, 10); printf("\n");
    printf("  ts  = %d, fn = %d, mn = %d, hnf = %d\n",
           v->ts, v->fn, v->mn, v->hnf);
    printf("  dir = %d, cc = %d, cn = %d, la  = %d\n",
           v->dir, v->cc, v->cn, v->la);
    printf("  → K = "); print_hex(v->K, 10); printf("\n");
    printf("  → R = "); print_hex(v->R, 8);  printf("\n");
    printf("\n");
}

static void stampa_vettore_tea5(const VettoreTEA5 *v)
{
    printf("  [%s]\n", v->nome);

    printf("  CK   = "); print_hex(v->ck, 24); printf("\n");
    printf("  ts   = %d, fn = %d, mn  = %d, hnf = %d\n",
           v->ts, v->fn, v->mn, v->hnf);
    printf("  dir  = %d, cc = %d, cn  = %d, la  = %d\n",
           v->dir, v->cc, v->cn, v->la);
    printf("  subs = %d, pdu = %d\n", v->subs, v->pdu);
    printf("  → CKM = "); print_hex(v->ckm, 24); printf("\n");
    printf("  → IVM = "); print_hex(v->ivm, 24); printf("\n");
    printf("\n");
}


int main(void)
{
    srand((unsigned int)time(NULL));

   // TEA2 

    VettoreTEA2 tea2_vettori[3];

   // C1  valori minimi 
    tea2_vettori[0].nome = "C1 minimi";
    memset(tea2_vettori[0].ck, 0x00, 10);
    tea2_vettori[0].ts  = 1;
    tea2_vettori[0].fn  = 1;
    tea2_vettori[0].mn  = 1;
    tea2_vettori[0].hnf = 0;
    tea2_vettori[0].dir = 0;
    tea2_vettori[0].cc  = 0;
    tea2_vettori[0].cn  = 0;
    tea2_vettori[0].la  = 0;

    // C2 casuali
    tea2_vettori[1].nome = "C2 casuali";
    gen_ck_random(tea2_vettori[1].ck, 10);
    tea2_vettori[1].ts  = (rand() % 4)     + 1;
    tea2_vettori[1].fn  = (rand() % 18)    + 1;
    tea2_vettori[1].mn  = (rand() % 60)    + 1;
    tea2_vettori[1].hnf =  rand() % 32768;
    tea2_vettori[1].dir =  rand() % 2;
    tea2_vettori[1].cc  =  rand() % 64;
    tea2_vettori[1].cn  =  rand() % 4096;
    tea2_vettori[1].la  =  rand() % 16384;

    // C3  valori massimi 
    tea2_vettori[2].nome = "C3 massimi";
    memset(tea2_vettori[2].ck, 0xFF, 10);
    tea2_vettori[2].ts  = 4;
    tea2_vettori[2].fn  = 18;
    tea2_vettori[2].mn  = 60;
    tea2_vettori[2].hnf = 32767;
    tea2_vettori[2].dir = 1;
    tea2_vettori[2].cc  = 63;
    tea2_vettori[2].cn  = 4094;
    tea2_vettori[2].la  = 16383;

    printf("=== VETTORI TEA2 ===\n\n");
    for (int i = 0; i < 3; i++) {
        calcola_stato_tea2(&tea2_vettori[i]);
        stampa_vettore_tea2(&tea2_vettori[i]);
    }

   // TEA5 

    VettoreTEA5 tea5_vettori[3];

   // C1 valori minimi 
    tea5_vettori[0].nome = "C1 minimi";
    memset(tea5_vettori[0].ck, 0x00, 24);
    tea5_vettori[0].ts   = 1;
    tea5_vettori[0].fn   = 1;
    tea5_vettori[0].mn   = 1;
    tea5_vettori[0].hnf  = 0;
    tea5_vettori[0].dir  = 0;
    tea5_vettori[0].cc   = 0;
    tea5_vettori[0].cn   = 0;
    tea5_vettori[0].la   = 0;
    tea5_vettori[0].subs = 0;
    tea5_vettori[0].pdu  = 0;

    // C2 casuali 
    tea5_vettori[1].nome = "C2 - casuali";
    gen_ck_random(tea5_vettori[1].ck, 24);
    tea5_vettori[1].ts   = (rand() % 4)     + 1;
    tea5_vettori[1].fn   = (rand() % 18)    + 1;
    tea5_vettori[1].mn   = (rand() % 60)    + 1;
    tea5_vettori[1].hnf  =  rand() % 65536;
    tea5_vettori[1].dir  =  rand() % 2;
    tea5_vettori[1].cc   =  rand() % 64;
    tea5_vettori[1].cn   =  rand() % 4096;
    tea5_vettori[1].la   =  rand() % 16384;
    tea5_vettori[1].subs =  rand() % 2;
    tea5_vettori[1].pdu  =  rand() % 256;

    // C3 valori massimi 
    tea5_vettori[2].nome = "C3 massimi";
    memset(tea5_vettori[2].ck, 0xFF, 24);
    tea5_vettori[2].ts   = 4;
    tea5_vettori[2].fn   = 18;
    tea5_vettori[2].mn   = 60;
    tea5_vettori[2].hnf  = 65535;
    tea5_vettori[2].dir  = 1;
    tea5_vettori[2].cc   = 63;
    tea5_vettori[2].cn   = 4095;
    tea5_vettori[2].la   = 16383;
    tea5_vettori[2].subs = 1;
    tea5_vettori[2].pdu  = 255;

    printf("=== VETTORI TEA5 ===\n\n");
    for (int i = 0; i < 3; i++) {
        calcola_stato_tea5(&tea5_vettori[i]);
        stampa_vettore_tea5(&tea5_vettori[i]);
    }

    return EXIT_SUCCESS;
}
