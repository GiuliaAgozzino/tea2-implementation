#include "utils.h"


static void usage_tea2(const char *prog)
{
    fprintf(stderr,
        "Uso: %s TEA2 CK nByte ts fn mn hnf dir cc cn la\n"
        "  CK   : chiave 80 bit in hex (20 cifre, prefisso 0x opzionale)\n"
        "  nByte: byte di key stream richiesti [>= 1]\n"
        "  ts   : timeslot        [1-4]\n"
        "  fn   : frame           [1-18]\n"
        "  mn   : multiframe      [1-60]\n"
        "  hnf  : hyperframe      [0-32767]\n"
        "  dir  : direction       [0=DL, 1=UL]\n"
        "  cc   : colour code     [0-63]\n"
        "  cn   : carrier number  [0-4095]\n"
        "  la   : location area   [0-16383]\n",
        prog);
}

static void usage_tea5(const char *prog)
{
    fprintf(stderr,
        "Uso: %s TEA5 CK nByte ts fn mn hnf dir cc cn la subs pdu\n"
        "  CK   : chiave 192 bit in hex (48 cifre, prefisso 0x opzionale)\n"
        "  nByte: byte di key stream richiesti [>= 1]\n"
        "  ts   : timeslot        [1-4]\n"
        "  fn   : frame           [1-18]\n"
        "  mn   : multiframe      [1-60]\n"
        "  hnf  : hyperframe      [0-65535]\n"
        "  dir  : direction       [0=DL, 1=UL]\n"
        "  cc   : colour code     [0-63]\n"
        "  cn   : carrier number  [0-4095]\n"
        "  la   : location area   [0-16383]\n"
        "  subs : subslot         [0-1]\n"
        "  pdu  : PDU counter     [0-255]\n",
        prog);
}

/* Converte stringa in intero con controllo errori.
 * Termina il programma con messaggio se la stringa non è un intero valido
 * o se il valore è fuori dall'intervallo [min, max]. */
static int parse_int(const char *s, const char *name, int min, int max)
{
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);

    if (errno != 0 || *end != '\0') {
        fprintf(stderr, "Errore: '%s' non è un intero valido per '%s'\n", s, name);
        exit(EXIT_FAILURE);
    }
    if (v < min || v > max) {
        fprintf(stderr, "Errore: '%s' deve essere compreso tra %d e %d (ricevuto: %ld)\n",
                name, min, max, v);
        exit(EXIT_FAILURE);
    }
    return (int)v;
}


int main(int argc, char *argv[])
{
    /* Controlla che ci sia almeno l'argomento dell'algoritmo */
    if (argc < 2) {
        fprintf(stderr, "Errore: specificare 'TEA2' o 'TEA5' come primo argomento.\n");
        return EXIT_FAILURE;
    }

    int is_tea2 = (strcmp(argv[1], "TEA2") == 0);
    int is_tea5 = (strcmp(argv[1], "TEA5") == 0);

    if (!is_tea2 && !is_tea5) {
        fprintf(stderr, "Errore: il primo argomento deve essere 'TEA2' o 'TEA5'\n");
        return EXIT_FAILURE;
    }

    /* Controlla il numero di argomenti attesi */
    if (is_tea2 && argc != 12) { usage_tea2(argv[0]); return EXIT_FAILURE; }
    if (is_tea5 && argc != 14) { usage_tea5(argv[0]); return EXIT_FAILURE; }

    /* Validazione chiave */
    const char *ck_hex   = argv[2];
    size_t      ck_len   = strlen(ck_hex);
    int         ck_prefix = (ck_len >= 2 && ck_hex[0] == '0' &&
                              (ck_hex[1] == 'x' || ck_hex[1] == 'X')) ? 2 : 0;
    size_t      ck_hex_digits = ck_len - (size_t)ck_prefix;

    if (is_tea2 && ck_hex_digits != 20) {
        fprintf(stderr, "Errore: CK per TEA2 deve essere 80 bit (20 cifre hex)\n");
        return EXIT_FAILURE;
    }
    if (is_tea5 && ck_hex_digits != 48) {
        fprintf(stderr, "Errore: CK per TEA5 deve essere 192 bit (48 cifre hex)\n");
        return EXIT_FAILURE;
    }

    /* Parsing argomenti comuni */
    int num_bytes = parse_int(argv[3],  "nByte", 1,     65536);
    int ts        = parse_int(argv[4],  "ts",    1,     4    );
    int fn        = parse_int(argv[5],  "fn",    1,     18   );
    int mn        = parse_int(argv[6],  "mn",    1,     60   );
    int hnf_max   = is_tea5 ? 65535 : 32767;
    int hnf       = parse_int(argv[7],  "hnf",   0,     hnf_max);
    int dir       = parse_int(argv[8],  "dir",   0,     1    );
    int cc        = parse_int(argv[9],  "cc",    0,     63   );
    int cn        = parse_int(argv[10], "cn",    0,     4095 );
    int la        = parse_int(argv[11], "la",    0,     16383);

    /* Parsing argomenti specifici TEA5 */
    int subs = 0, pdu = 0;
    if (is_tea5) {
        subs = parse_int(argv[12], "subs", 0, 1  );
        pdu  = parse_int(argv[13], "pdu",  0, 255);
    }

    /* Generazione key stream */
    uint8_t *ks = malloc((size_t)num_bytes);
    if (ks == NULL) {
        fprintf(stderr, "Errore: allocazione memoria fallita\n");
        return EXIT_FAILURE;
    }

    if (is_tea2) {
        generateKeyStreamTEA2(ks, ck_hex, num_bytes,
                               ts, fn, mn, hnf, dir, cc, cn, la);
    } else {
        generateKeyStreamTEA5(ks, ck_hex, num_bytes,
                               ts, fn, mn, hnf, dir, subs, cn, la, cc, pdu);
    }

    /* Stampa */
    printf("Key stream (%d byte):\n", num_bytes);
    for (int i = 0; i < num_bytes; i++) {
        printf("%02X", ks[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (num_bytes % 16 != 0) printf("\n");

    free(ks);
    return EXIT_SUCCESS;
}