#include "cryptopals/hamming_distance.h"
#include "cryptopals/format_conversions.h"
#include "cryptopals/rw_ops.h"
#include "cryptopals/repeating_key_xor.h"

#include "cryptopals/aes_ecb.h"
#include "cryptopals/ossl_core.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    init_openssl();

    char* file_buf_base64 = get_string_from_file("enc_file.txt", 0);
    char file_buf[4096] = {0};
    int file_buf_len = 4096;

    if (base64_to_bin(file_buf_base64, file_buf, &file_buf_len) == 0)
        return 1;

    const unsigned char key[] = "YELLOW SUBMARINE";
    unsigned char decrypted_file[4096] = {0};
    int dec_len = 4096;

    if (1 != decrypt_aes_128_ecb(key, (const unsigned char *)file_buf,
                                 file_buf_len, decrypted_file, &dec_len))
        return 1;

    // result
    /*
    printf("decrypted file length: %d\n", total_decrypted_len);
    write(1, decrypted_file, total_decrypted_len);
    */
    free(file_buf_base64);
    return 0;
}
