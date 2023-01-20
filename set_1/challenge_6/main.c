#include "cryptopals/hamming_distance.h"
#include "cryptopals/rw_ops.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    if (hamming_distance("this is a test", 14, "wokka wokka!!!", 14) != 37)
        return 0;

    char* file_buf = get_string_from_file("enc_file.txt", 0);

    printf("file_buf : %s", file_buf);

    free(file_buf); 
    return 0;
}