#include "cryptopals/hamming_distance.h"
#include "cryptopals/format_conversions.h"
#include "cryptopals/rw_ops.h"
#include "cryptopals/repeating_key_xor.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/conf.h>

int main() {

    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);

    char* file_buf_base64 = get_string_from_file("enc_file.txt", 0);
    char file_buf[4096] = {0};
    int file_buf_len = 4096;

    if (base64_to_bin(file_buf_base64, file_buf, &file_buf_len) == 0)
        return 1;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const unsigned char key[] = "YELLOW SUBMARINE";

    unsigned char decrypted_file[4096] = {0};
    int dec_len = 4096;
    int dec_len_save = dec_len;
    int total_decrypted_len = 0;

    EVP_DecryptInit(ctx, EVP_aes_128_ecb(), key, NULL);
    EVP_DecryptUpdate(ctx, decrypted_file, &dec_len,
                    (const unsigned char*)file_buf, file_buf_len);
    total_decrypted_len += dec_len;
    int bytes_left = dec_len_save - dec_len;
    EVP_DecryptFinal(ctx, decrypted_file + dec_len, &bytes_left);
    total_decrypted_len += bytes_left;

    // result
    /*
    printf("decrypted file length: %d\n", total_decrypted_len);
    write(1, decrypted_file, total_decrypted_len);
    */
    EVP_CIPHER_CTX_free(ctx);
    free(file_buf_base64);
    return 0;
}
