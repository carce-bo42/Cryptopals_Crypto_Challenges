#include "cryptopals/aes_cbc.h"
#include "cryptopals/rw_ops.h"
#include "cryptopals/format_conversions.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main() {
    char* file_buf_base64 = get_string_from_file("enc_file.txt", 0);
    char file_buf[4096] = {0};
    int file_buf_len = 4096;

    const unsigned char *key = (const unsigned char *)"YELLOW SUBMARINE";
    const unsigned char *iv =  (const unsigned char *)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    unsigned char decrypted_file[8192] = {0};
    int dec_len = 8192;

    if (base64_to_bin(file_buf_base64, file_buf, &file_buf_len) == 0)
        return 1;
    
    if (1 != decrypt_aes_128_cbc(key, iv, (const unsigned char *)file_buf,
                                 file_buf_len, decrypted_file, &dec_len, 0))
    {
        printf("wtffff\n");
        return 1;
    }

    /* Spoils the result :
    printf("\n");
    write(1, decrypted_file, dec_len);    
    printf("\n");    
    */

    free(file_buf_base64);
    return 0;
}
