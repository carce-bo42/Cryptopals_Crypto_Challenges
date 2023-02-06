#include "cryptopals/aes_cbc.h"
#include "cryptopals/aes_ecb.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    const unsigned char* str = (const unsigned char *)"Hello World 0123";
    const unsigned char* key = (const unsigned char *)"0123456789ABCDEF";
    const unsigned char* iv = (const unsigned char *)"FEDCBA9876543210";
    unsigned char out[16] = {0};
    int out_len = 16;
    unsigned char decrypted[16] = {0};
    int decrypted_len = 16;

    /* This works :
    (void)iv;
    if (encrypt_aes_128_ecb(key, str, 16, out, &out_len) != 1)
        return 1;
    if (decrypt_aes_128_ecb(key, (const unsigned char *)out,
                            out_len, decrypted, &decrypted_len) != 1)
        return 1;
    */

    /* This does not : 
    if (encrypt_aes_128_cbc(key, iv, str, 16, out, &out_len) != 1)
        return 1;
    if (decrypt_aes_128_cbc(key, iv, (const unsigned char *)out,
                            out_len, decrypted, &decrypted_len) != 1)
        return 1;*/

    printf("\n");
    write(1, decrypted, decrypted_len);
    printf("\n");
    return 0;
}
