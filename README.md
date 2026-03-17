# tea2-tea5-implementation


# Per compilare generate_vectors.c

gcc -o generate_vectors     generate_vectors.c     
TEA2/tb5.c     
TEA2/iv_loading.c     
TEA2/eck_loading.c     
TEA5/iv_expander.c     
TEA5/function_f.c     
-I.


# Per compilare il main.c e stampare il ks generato 

gcc main.c  generate_ks.c  TEA2/*.c TEA5/*.c TEA5/RIJNDAEL/*.c -o generate_ks



# Per compilare benchmark_tea2.c, in output si ottiene bench_tea2_results.csv

gcc -O2 -o bench_tea2 benchmark_tea2.c \
    TEA2/eck_loading.c \
    TEA2/expander_E.c \
    TEA2/function_f1_f2.c \
    TEA2/iv_loading.c \
    TEA2/permutation_BP.c \
    TEA2/permutation_P.c \
    TEA2/tb5.c \
    TEA2/tea2.c \
    -I. -lm

# Per compilare benchmark_tea5.c, in output si ottiene bench_tea5_results.csv

gcc -O2 -o bench_tea5 benchmark_tea5.c \
    TEA5/function_f.c \
    TEA5/iv_expander.c \
    TEA5/tea5.c \
    TEA5/RIJNDAEL/addRoundKey.c \
    TEA5/RIJNDAEL/keyExpansion.c \
    TEA5/RIJNDAEL/mixColumn.c \
    TEA5/RIJNDAEL/rijndael.c \
    TEA5/RIJNDAEL/rijndael_tables.c \
    TEA5/RIJNDAEL/shiftRow.c \
    TEA5/RIJNDAEL/subByte.c \
    -I. -lm