#include <openssl/evp.h>
#include <openssl/conf.h>

/*
 * Because the key length is fixed for AES-128-ECB, the signature of
 * EVP_Decrypt/EncryptInit has no parameter keylen.
 * The key length MUST be 128 bits.
 */

int decrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len,
                        int padding)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int dec_len = 0;
    int ret = 0;

    if (1 != EVP_DecryptInit(ctx, EVP_aes_128_ecb(), key, NULL)) {
        printf("Decrypt Init ecb error\n");
        goto err;
    }
    EVP_CIPHER_CTX_set_padding(ctx, padding);

    while (dec_len < in_len) {
        if (1 != EVP_DecryptUpdate(ctx, out + dec_len, out_len,
                                (const unsigned char*)in + dec_len, 16))
        {
            printf("Decrypt Update ecb error\n");
            goto err;
        }

        dec_len += 16;
        *out_len -= 16;
    }

    if (1 != EVP_DecryptFinal(ctx, (unsigned char *)out + dec_len,
                              out_len))
    {
        printf("Decrypt Final ecb error\n");
        goto err;
    }

    *out_len += dec_len;
    ret = 1;

err:
    if (ctx) { EVP_CIPHER_CTX_free(ctx); }
    return ret;
}

int encrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len,
                        int padding)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int enc_len = 0;
    int ret = 0;

    if (1 != EVP_EncryptInit(ctx, EVP_aes_128_ecb(), key, NULL))
    {
        printf("Encrypt Init ecb error\n");
        goto err;
    }
    EVP_CIPHER_CTX_set_padding(ctx, padding);

    while (enc_len < in_len) {

        if (1 != EVP_EncryptUpdate(ctx, out + enc_len, out_len,
                                (const unsigned char*)in + enc_len, 16))
        {
            printf("Encrypt Update ecb error\n");
            goto err;
        }

        enc_len += 16;
        *out_len -= 16;
    }

    if (1 != EVP_EncryptFinal(ctx, (unsigned char *)out + enc_len,
                                   out_len))
    {
        printf("Encrypt Final ecb error\n");
        goto err;
    }

    *out_len += enc_len;
    ret = 1;

err:
    if (ctx) { EVP_CIPHER_CTX_free(ctx); }
    return ret;
}