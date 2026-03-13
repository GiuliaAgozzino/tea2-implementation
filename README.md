# tea2-tea5-implementation


Per compilare generate_vectors

gcc -o generate_vectors     generate_vectors.c     
TEA2/tb5.c     
TEA2/iv_loading.c     
TEA2/eck_loading.c     
TEA5/iv_expander.c     
TEA5/function_f.c     
-I.


Per compilare il main e stampare il ks generato 

 gcc main.c  generate_ks.c  TEA2/*.c TEA5/*.c TEA5/RIJNDAEL/*.c -o generate_ks