#include <openssl/evp.h>
#include <openssl/conf.h>

/*
 * Because the key length is fixed for AES-128-ECB, the signature of
 * EVP_Decrypt/EncryptInit has no parameter keylen.
 * The key length MUST be 128 bits.
 */

int decrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int out_len_cpy = *out_len;
    int total_decrypted_len = 0;
    int bytes_left = 0;
    int ret = 0;

    if (1 != EVP_DecryptInit(ctx, EVP_aes_128_ecb(), key, NULL))
        goto err;

    if (1 != EVP_DecryptUpdate(ctx, out, &out_len_cpy,
                               (const unsigned char*)in, in_len))
        goto err;
    
    total_decrypted_len += out_len_cpy;
    bytes_left = *out_len - total_decrypted_len;

    if (1 != EVP_DecryptFinal(ctx, (unsigned char *)in + total_decrypted_len,
                                   &bytes_left))
        goto err;

    total_decrypted_len += bytes_left;
    *out_len = total_decrypted_len;
    ret = 1;

err:
    EVP_CIPHER_CTX_free(ctx);
    return ret;
}

int encrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int out_len_cpy = *out_len;
    int total_encrypted_len = 0;
    int bytes_left = 0;
    int ret = 0;

    if (1 != EVP_EncryptInit(ctx, EVP_aes_128_ecb(), key, NULL))
        goto err;

    if (1 != EVP_EncryptUpdate(ctx, out, &out_len_cpy,
                               (const unsigned char*)in, in_len))
        goto err;
    
    total_encrypted_len += out_len_cpy;
    int bytes_left = *out_len - total_encrypted_len;

    if (1 != EVP_EncryptFinal(ctx, (unsigned char *)in + total_encrypted_len,
                                   &bytes_left))
        goto err;

    total_encrypted_len += bytes_left;
    *out_len = total_encrypted_len;
    ret = 1;

err:
    EVP_CIPHER_CTX_free(ctx);
    return ret;
}