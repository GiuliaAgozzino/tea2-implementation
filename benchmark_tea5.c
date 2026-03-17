//TEST: TEA5

#define _POSIX_C_SOURCE 199309L

#include "utils.h"
#include "TEA5/RIJNDAEL/rijndael.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N_ITER   200
#define N_WARMUP 1

static const uint32_t BYTE_SIZES[] = { 128, 1024, 1048576 };
#define N_SIZES 3

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
// Misurazioni

// wall_us: tempo reale (wall clock) in microsecondi

static double wall_us(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec * 1e6 + (double)t.tv_nsec * 1e-3;
}

// cpu_us : tempo CPU in microsecondi

static double cpu_us(void)
{
    struct timespec t;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    return (double)t.tv_sec * 1e6 + (double)t.tv_nsec * 1e-3;
}

// hwm_kb : picco massimo storico di memoria fisica usata dal processo (High Water Mark)
//          a differenza di VmRSS (valore istantaneo), VmHWM non scende mai:
//          rappresenta il footprint massimo raggiunto dall'avvio del processo.

static long hwm_kb(void)
{
    long hwm = -1;
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return hwm;
    char line[128];
    while (fgets(line, sizeof line, f)) {
        if (strncmp(line, "VmHWM:", 6) == 0) {
            sscanf(line + 6, "%ld", &hwm);
            break;
        }
    }
    fclose(f);
    return hwm;
}

// Configurazioni di test C1, C2, C3

typedef struct {
    const char *name;
    const char *ck_hex;   // 48 cifre hex = 192 bit
    int ts, fn, mn, hnf, dir, cc, cn, la, subs, pdu;
} Config;

static const Config CONFIGS[] = {
    /* C1: valori minimi
    CKM = 5AD39D93..C392C170   IVM = E5821812..321C36B9 */
    { "C1_min",
      "00000000" "00000000" "00000000"
      "00000000" "00000000" "00000000",
      1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },

    /* C2: valori casuali
    CKM = EEACB6A8..C657D5C8   IVM = FEFB938C..29DE8E57 */
    { "C2_random",
      "E82F359E" "E9541140" "B01ACB41"
      "5C405FB0" "0F5F21A4" "CBBB00E7",
      2, 12, 55, 26519, 0, 42, 417, 13177, 0, 215 },

    /* C3: valori massimi — cn = 4095 (nessun problema ECK in TEA5)
    CKM = DA5DC43E..EF0FD01E   IVM = ECFEBD80..0232E370 */
    { "C3_max",
      "FFFFFFFF" "FFFFFFFF" "FFFFFFFF"
      "FFFFFFFF" "FFFFFFFF" "FFFFFFFF",
      4, 18, 60, 65535, 1, 63, 4095, 16383, 1, 255 },
};
#define N_CONFIGS 3

//Scrittura CSV con i risultati:
//  config : nome della configurazione (C1_min, C2_random, C3_max)
//  nb     : numero di byte di keystream generati
//  iter   : indice dell'iterazione corrente (0 ... N_ITER-1)
//  wp     : wall-clock fase preparazione chiave+IV       [µs]
//  wg     : wall-clock fase generazione keystream        [µs]
//  wt     : wall-clock totale (wp + wg)                  [µs]
//  cp     : CPU time fase preparazione chiave+IV         [µs]
//  cg     : CPU time fase generazione keystream          [µs]
//  ct     : CPU time totale (cp + cg)                    [µs]
//  hwm    : High Water Mark memoria al termine della chiamata  [kB]
//  tput   : throughput = nb / wt                         [MB/s]

static void write_row(FILE *out, const char *config, uint32_t nb, int iter,
                      double wp, double wg, double wt,
                      double cp, double cg, double ct,
                      long hwm, double tput)
{
    fprintf(out,
            "TEA5,%s,%u,%d,"
            "%.3f,%.3f,%.3f,"
            "%.3f,%.3f,%.3f,"
            "%ld,%.6f\n",
            config, nb, iter,
            wp, wg, wt,
            cp, cg, ct,
            hwm, tput);
}


static void bench(FILE *out, const Config *cfg, uint32_t nb, int iter, int wr)
{
    uint8_t *ks = malloc(nb);
    if (!ks) { fputs("OOM\n", stderr); exit(EXIT_FAILURE); }

    const IvTEA5 ivc = {
        .ts   = (uint8_t)  cfg->ts,
        .fn   = (uint8_t)  cfg->fn,
        .mn   = (uint8_t)  cfg->mn,
        .hnf  = (uint16_t) cfg->hnf,
        .dir  = (uint8_t)  cfg->dir,
        .subs = (uint8_t)  cfg->subs,
        .cn   = (uint16_t) cfg->cn,
        .la   = (uint16_t) cfg->la,
        .cc   = (uint8_t)  cfg->cc,
        .pdu  = (uint8_t)  cfg->pdu,
    };

    // Misurazione tempo preparazione chiave e IV
    double w0 = wall_us(), c0 = cpu_us();

    uint8_t  ck[24];
    hex_to_bytes(cfg->ck_hex, ck, 24);
    uint80_t iv = formIVTEA5(ivc);
    uint8_t  ivx[24];
    iv_expander(iv, ivx);
    uint8_t  ckm[24], ivm[24];
    f(ck, ivx, ckm, ivm);

    // Misurazione tempo generazione keystream
    double w1 = wall_us(), c1 = cpu_us();

    tea5(nb, ckm, ivm, ks);

    double w2 = wall_us(), c2 = cpu_us();

    // w1 - w0 e c1 - c0  costo della preparazione
    // w2 - w1 e c2 - c1  costo della generazione
    // w2 - w0 e c2 - c0  costo totale

    if (wr) {
        long   hwm  = hwm_kb();
        double wtot = w2 - w0;
        double tput = (wtot > 0.0) ? (double)nb / wtot : 0.0;
        write_row(out, cfg->name, nb, iter,
                  w1-w0, w2-w1, wtot,
                  c1-c0, c2-c1, c2-c0,
                  hwm, tput);
    }

    free(ks);
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <config> <num_bytes>\n", argv[0]);
        fprintf(stderr, "  config   : C1_min | C2_random | C3_max\n");
        fprintf(stderr, "  num_bytes: 128 | 1024 | 1048576\n");
        return EXIT_FAILURE;
    }

    // trova la configurazione
    const Config *cfg = NULL;
    for (int ci = 0; ci < N_CONFIGS; ci++) {
        if (strcmp(CONFIGS[ci].name, argv[1]) == 0) {
            cfg = &CONFIGS[ci];
            break;
        }
    }
    if (!cfg) { fprintf(stderr, "Config non trovata: %s\n", argv[1]); return EXIT_FAILURE; }

    uint32_t nb = (uint32_t)atol(argv[2]);

    // apre il CSV in append così tutti i processi scrivono sullo stesso file
    FILE *out = fopen("bench_tea5_results.csv", "a");
    if (!out) { perror("fopen"); return EXIT_FAILURE; }

    // scrive l'intestazione solo se il file è vuoto
    fseek(out, 0, SEEK_END);
    if (ftell(out) == 0)
        fputs("algo,config,num_bytes,iter,"
              "wall_prep_us,wall_gen_us,wall_total_us,"
              "cpu_prep_us,cpu_gen_us,cpu_total_us,"
              "hwm_kb,throughput_MBps\n", out);

    printf("[TEA5] %s  %u B  warmup...", cfg->name, nb);
    fflush(stdout);
    for (int w = 0; w < N_WARMUP; w++)
        bench(out, cfg, nb, -1, 0);

    printf("  %d iter\n", N_ITER);
    for (int it = 0; it < N_ITER; it++)
        bench(out, cfg, nb, it, 1);

    fclose(out);
    return EXIT_SUCCESS;
}