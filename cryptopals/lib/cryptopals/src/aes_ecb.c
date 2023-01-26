#include <openssl/evp.h>
#include <openssl/conf.h>

int decrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int out_len_cpy = *out_len;
    int total_decrypted_len = 0;

    if (1 != EVP_DecryptInit(ctx, EVP_aes_128_ecb(), key, NULL))
        return 1;

    if (1 != EVP_DecryptUpdate(ctx, out, &out_len_cpy,
                               (const unsigned char*)in, in_len))
        return 1;
    
    total_decrypted_len += out_len_cpy;
    int bytes_left = *out_len - total_decrypted_len;

    if (1 != EVP_DecryptFinal(ctx, (unsigned char *)in + total_decrypted_len,
                                   &bytes_left))
        return 1;
    total_decrypted_len += bytes_left;

    *out_len = total_decrypted_len;
    return 0;
}