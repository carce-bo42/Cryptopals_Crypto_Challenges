#include "cryptopals/hamming_distance.h"
#include "cryptopals/format_conversions.h"
#include "cryptopals/rw_ops.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYSIZE 80

int get_min_val_entry(float* mat, int size) {

    int min_val_entry = 0;

    for (int i = 0; i < size; i++) {
        if (mat[i] < mat[min_val_entry])
            min_val_entry = i;
    }
    return min_val_entry;
}

int main() {

    if (hamming_distance("this is a test", 14, "wokka wokka!!!", 14) != 37)
        return 0;

    char* file_buf_base64 = get_string_from_file("enc_file.txt", 0);
    char file_buf[4096] = {0};
    int file_buf_len = 4096;

    if (base64_to_bin(file_buf_base64, file_buf, &file_buf_len) == 0)
        return 1;

    float hd_keysizes[78] = {0};
    for (int keysize = 2; keysize < MAX_KEYSIZE; keysize++) {
        hd_keysizes[keysize - 2] = (float)hamming_distance(file_buf, keysize,
                                                file_buf + keysize, keysize)
                                / (float)keysize;
        //printf("keysize : %d, hd_keysize : %f\n", keysize, hd_keysizes[keysize]);
    }
    int min_val_entry = get_min_val_entry(hd_keysizes, 78);
    // raises keylen = 5
    //printf("keysize : %d, hamming_distance: %f\n", min_val_entry + 2, hd_keysizes[min_val_entry]);

    free(file_buf_base64); 
    return 0;
}