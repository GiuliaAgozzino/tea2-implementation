#include "utils.h"

void generateKeyStreamTEA2(
    uint8_t ks[],
     char *ck_hex,
    int num_byte_ks,
    int ts, int fn, int mn, int hnf,
    int dir, int cc, int cn, int la);

void generateKeyStreamTEA5(
    uint8_t ks[],
     char *ck_hex,
    int num_byte_ks,
    int ts, int fn, int mn, int hnf,
    int dir, int subs, int cn, int la,
    int cc, int pdu);

uint32_t formIVTEA2(ivStructTEA2 iv) {

    // Combina i componenti dell'IV in un unico numero a 32 bit
    return ((uint32_t)(iv.ts - 1)
          | (uint32_t)(iv.fn  << 2)
          | (uint32_t)(iv.mn  << 7)
          | (uint32_t)((iv.hnf & 0x7FFF) << 13)
          | (uint32_t)(iv.dir << 28));
}

uint80_t formIVTEA5(ivStructTEA5 iv) {

     // Combina i componenti dell'IV in un unico numero a 80 bit
    return (uint80_t)(iv.ts - 1)                  
         | ((uint80_t)iv.fn << 2)                 
         | ((uint80_t)iv.mn << 7)                
         | ((uint80_t)(iv.hnf & 0xFFFF) << 13)   
         | ((uint80_t)(iv.dir & 0x1) << 29)      
         | ((uint80_t)(iv.subs & 0x1) << 30)     
         | ((uint80_t)(iv.cn  & 0x0FFF) << 31)   
         | ((uint80_t)(iv.la  & 0x3FFF) << 43)    
         | ((uint80_t)(iv.cc  & 0x3F) << 57)      
         | ((uint80_t)(iv.pdu & 0xFF) << 63);     
         // IV(71-79) = 0 
}

uint80_t hex_to_uint80( char *s) {

    // Rimuove eventuale prefisso "0x" o "0X"
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    uint80_t result = 0;

    // Continua a leggere fino alla fine della stringa
    while (*s) {

        // Shift a sinistra di 4 bit per fare spazio al nuovo nibble
        result <<= 4;

        // Converte il carattere esadecimale in un valore numerico
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
     
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    if (strcmp(argv[1], "TEA2") != 0 && strcmp(argv[1], "TEA5") != 0) {
        printf("Errore: il primo argomento deve essere 'TEA2' o 'TEA5', per indicare l'algoritmo da utilizzare\n");
        return 1;
    }

    if (strcmp(argv[1], "TEA2") == 0 && argc != 12) {
        printf("Uso: ./programma TEA2/TEA5 CK nByte ts fn mn hnf dir cc cn la\n"
               "  TEA2 o TEA5: quale algoritmo usare la la generazione del key stream)\n"
               "  CK:  chiave di cifratura (hex, 80 bit)\n"
               "  numero di byte di key stream richiesto [>= 1]\n"
               "  ts:  timeslot        [1-4]\n"
               "  fn:  frame           [1-18]\n"
               "  mn:  multiframe      [1-60]\n"
               "  hnf: hyperframe      [0-65535]\n"
               "  dir: direction       [0-1]\n"
               "  cc:  colour code     [0-63]\n"
               "  cn:  carrier number  [0-4095]\n"
               "  la:  location area   [0-16383]\n"
               "\nNota: CK va fornita in esadecimale (20 cifre hex, 80 bit).\n"
       "      Il prefisso 0x/0X e' opzionale. Es: 0xA3F1...  oppure  A3F1...\n");
        return 1;
    }

    if (strcmp(argv[1], "TEA5") == 0 && argc != 14) {
        printf("Uso: ./programma TEA2/TEA5 CK nByte ts fn mn hnf dir cc cn la subs pdu \n"
               "  TEA2 o TEA5: quale algoritmo usare la la generazione del key stream)\n"
               "  CK:  chiave di cifratura (hex, 80 bit)\n"
               "  numero di byte di key stream richiesto [>= 1]\n"
               "  ts:  timeslot        [1-4]\n"
               "  fn:  frame           [1-18]\n"
               "  mn:  multiframe      [1-60]\n"
               "  hnf: hyperframe      [0-65535]\n"
               "  dir: direction       [0-1]\n"
               "  cc:  colour code     [0-63]\n"
               "  cn:  carrier number  [0-4095]\n"
               "  la:  location area   [0-16383]\n"
               " subs: subslot         [0-1]\n"
               " pdu:  protocol data unit counter [0-255]\n"
            "\nNota: CK va fornita in esadecimale (48 cifre hex, 80 bit).\n"
       "      Il prefisso 0x/0X e' opzionale. Es: 0xA3F1...  oppure  A3F1...\n");
        return 1;
    }
    

    // Validazione degli argomenti comuni (CK, numero di byte di key stream, ts, fn, mn, hnf, dir, cc, cn, la)
    
    if (strcmp(argv[1], "TEA2") == 0 && (strlen(argv[2]) > 22 || strlen(argv[2]) < 20) ) {
        printf("Errore: la chiave di cifratura deve essere un numero a 80 bit nel formato esadecimale (20 cifre hex, prefisso 0x opzionale)\n");
        return 1;
    }
    
    if (strcmp(argv[1], "TEA5") == 0 && (strlen(argv[2]) > 50 || strlen(argv[2]) < 48) ) {
        printf("Errore: la chiave di cifratura deve essere un numero a 192 bit nel formato esadecimale (46 cifre hex, prefisso 0x opzionale)\n");
        return 1;
    }

    int num_byte_ks = atoi(argv[3]);
    if (num_byte_ks < 1) {
        printf("Errore: numero di byte di key stream richiesto deve essere almeno 1\n");
        return 1;
    }

    int ts = atoi(argv[4]);
    if (ts < 1 || ts > 4) {
        printf("Errore: timeslot deve essere compreso tra 1 e 4\n");
        return 1;
    }

    int fn = atoi(argv[5]);
    if (fn < 1 || fn > 18) {
        printf("Errore: frame deve essere compreso tra 1 e 18\n");
        return 1;
    }

    int mn = atoi(argv[6]);
    if (mn < 1 || mn > 60) {
        printf("Errore: multiframe deve essere compreso tra 1 e 60\n");
        return 1;
    }

    int hnf = atoi(argv[7]);
    if (hnf < 0 || hnf > 0xFFFF) {
        printf("Errore: hyperframe deve essere compreso tra 0 e 65535\n");
        return 1;
    }

    int dir = atoi(argv[8]);
    if (dir != 0 && dir != 1) {
        printf("Errore: direction deve essere 0 o 1\n");
        return 1;
    }

    int cc = atoi(argv[9]);
    if (cc < 0 || cc > 0x3F) {
        printf("Errore: colour code deve essere compreso tra 0 e 63\n");
        return 1;
    }

    int cn = atoi(argv[10]);
    if (cn < 0 || cn > 0x0FFF) {
        printf("Errore: carrier number deve essere compreso tra 0 e 4095\n");
        return 1;
    }

    int la = atoi(argv[11]);
    if (la < 0 || la > 0x3FFF) {
        printf("Errore: location area identifier deve essere compreso tra 0 e 16383\n");
        return 1;
    }
     
    // Validazione degli argomenti specifici per TEA5 (subslot e pdu)
     if (strcmp(argv[1], "TEA5") == 0){

        if (atoi(argv[12]) < 0 || atoi(argv[12]) > 1) {
            printf("Errore: subslot deve essere 0 o 1\n");
            return 1;
        }

        if (atoi(argv[13]) < 0 || atoi(argv[13]) > 0xFF) {
            printf("Errore: pdu deve essere compreso tra 0 e 255\n");
            return 1;
        }
     }


   
    // Generazione key stream
    uint8_t ks[num_byte_ks];
    if (strcmp(argv[1], "TEA2") == 0) {
        generateKeyStreamTEA2(ks, argv[2], num_byte_ks,ts, fn, mn, hnf, dir, cc, cn, la);
    } else {
        generateKeyStreamTEA5(ks, argv[2], num_byte_ks, ts, fn, mn, hnf, dir, atoi(argv[12]), cn, la, cc, atoi(argv[13]));
    }


    printf("Key stream generato:\n");
    
    for (int i = 0; i < num_byte_ks; i++) {
    printf("%02X", ks[i]);
    if ((i+1)%16 == 0) printf("\n");
    }


    
    printf("\n");
    
    return 0;
}


void generateKeyStreamTEA2(uint8_t ks[], char *ck_hex, int num_byte_ks, int ts, int fn, int mn, int hnf, int dir, int cc, int cn, int la) {

     uint80_t ck = hex_to_uint80(ck_hex);
     ivStructTEA2 ivComponents = {ts, fn, mn, hnf, dir};


    // Generazione chiave di cifratura tramite l'agoritmo TB5 e composizione IV
    uint80_t eck = tb5(ck, cn, cc, la);
    uint32_t iv  = formIVTEA2(ivComponents);

    // Creo due array per il registro di chiave e il registro di output, e li inizializzo con ECK e IV 
    uint8_t R[8];
    uint8_t K[10];

    iv_loading(iv, R);
    eck_loading(eck, K);
    
    tea2(num_byte_ks, R, K, ks);
}


void hexToBytes( char *hex, uint8_t *out, int len) {

    if (hex[0]=='0' && (hex[1]=='x' || hex[1]=='X'))
        hex += 2;

    for (int i = 0; i < len; i++) {
        sscanf(hex + 2*i, "%2hhx", &out[i]);
    }
}

void generateKeyStreamTEA5(uint8_t ks[],  char *ck_hex, int num_byte_ks, int ts, int fn, int mn, int hnf, int dir, int subs, int cn, int la, int cc, int pdu) {

     uint8_t ck[24];
     hexToBytes(ck_hex, ck, 24);
     ivStructTEA5 ivComponents = {ts, fn, mn, hnf, dir, subs, cn, la, cc, pdu};
     uint80_t iv = formIVTEA5(ivComponents);
     tea5(num_byte_ks, ck, iv, ks);

}