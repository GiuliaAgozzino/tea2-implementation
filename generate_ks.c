#include "utils.h"

uint32_t formIV(ivStruct *iv) {
    return ((iv->ts - 1)
          | (iv->fn  << 2)
          | (iv->mn  << 7)
          | ((iv->hnf & 0x7FFF) << 13)
          | (iv->dir << 28));
}

uint80_t hex_to_uint80(const char *s) {
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    uint80_t result = 0;
    while (*s) {
        result <<= 4;
        if      (*s >= '0' && *s <= '9') result |= *s - '0';
        else if (*s >= 'a' && *s <= 'f') result |= *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F') result |= *s - 'A' + 10;
        else {
            printf("Errore: carattere non valido nella chiave: %c\n", *s);
            exit(1);
        }
        s++;
    }
    return result;
}

int main(int argc, char *argv[]) {

    if (argc != 11) {
        printf("Uso: ./programma CK ts fn mn hnf dir cc cn la\n"
               "  CK:  chiave di cifratura (hex, 80 bit)\n"
               "  ts:  timeslot        [1-4]\n"
               "  fn:  frame           [1-18]\n"
               "  mn:  multiframe      [1-60]\n"
               "  hnf: hyperframe      [0-65535]\n"
               "  dir: direction       [0-1]\n"
               "  cc:  colour code     [0-63]\n"
               "  cn:  carrier number  [0-4095]\n"
               "  la:  location area   [0-16383]\n"
               "  numero di byte di key stream richiesto [>= 1]\n");
        return 1;
    }

    if (strlen(argv[1]) > 22) {
        printf("Errore: chiave di cifratura deve essere un numero a 80 bit\n");
        return 1;
    }

    int ts = atoi(argv[2]);
    if (ts < 1 || ts > 4) {
        printf("Errore: timeslot deve essere compreso tra 1 e 4\n");
        return 1;
    }

    int fn = atoi(argv[3]);
    if (fn < 1 || fn > 18) {
        printf("Errore: frame deve essere compreso tra 1 e 18\n");
        return 1;
    }

    int mn = atoi(argv[4]);
    if (mn < 1 || mn > 60) {
        printf("Errore: multiframe deve essere compreso tra 1 e 60\n");
        return 1;
    }

    int hnf = atoi(argv[5]);
    if (hnf < 0 || hnf > 0xFFFF) {
        printf("Errore: hyperframe deve essere compreso tra 0 e 65535\n");
        return 1;
    }

    int dir = atoi(argv[6]);
    if (dir != 0 && dir != 1) {
        printf("Errore: direction deve essere 0 o 1\n");
        return 1;
    }

    int cc = atoi(argv[7]);
    if (cc < 0 || cc > 0x3F) {
        printf("Errore: colour code deve essere compreso tra 0 e 63\n");
        return 1;
    }

    int cn = atoi(argv[8]);
    if (cn < 0 || cn > 0x0FFF) {
        printf("Errore: carrier number deve essere compreso tra 0 e 4095\n");
        return 1;
    }

    int la = atoi(argv[9]);
    if (la < 0 || la > 0x3FFF) {
        printf("Errore: location area identifier deve essere compreso tra 0 e 16383\n");
        return 1;
    }

    int num_byte_ks = atoi(argv[10]);
    if (num_byte_ks < 1) {
        printf("Errore: numero di byte di key stream richiesto deve essere almeno 1\n");
        return 1;
    }

    uint80_t ck = hex_to_uint80(argv[1]);
    ivStruct ivComponents = {ts, fn, mn, hnf, dir};

    uint80_t eck = tb5(ck, cn, cc, la);
    uint32_t iv  = formIV(&ivComponents);

    uint8_t R[8];
    iv_loading(iv, R);

    uint8_t K[10];
    eck_loading(eck, K);
    
    uint8_t ks[num_byte_ks];
    tea2(num_byte_ks, R, K, ks);
   
    printf("Key stream generato:\n");

    for (int i = 0; i < num_byte_ks; i++) {
    printf("%02X", ks[i]);
   }
    
    printf("\n");
    
    return 0;
}