#include "cryptopals/aes_cbc.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

    const unsigned char* str = (const unsigned char *)"Hello World 0123H";
    const unsigned char* key = (const unsigned char *)"0123456789ABCDEF";
    const unsigned char* iv = (const unsigned char *)"FEDCBA9876543210";
    unsigned char out[16] = {0};
    int out_len = 16;

    if (encrypt_aes_128_cbc(key, iv, str, strlen((const char *)str), out, &out_len) != 1)
        return 1;

    printf("\n");
    write(1, out, out_len);
    printf("\n");
    return 0;
}