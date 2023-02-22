#include "cryptopals/aes_ecb.h"
#include "cryptopals/logic_gates.h"
#include <string.h>
#include <stdio.h>

/*
 * Much in the same way as the signature of AES-128-ECB methods, 
 * An ivlen parameter is not needed since the the ivlen is fixed.
 * The iv length MUST be 
 */

int encrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len,
                        int padding)
{
    int enc_len = 0;
    unsigned char last_result[48] = {0};
    unsigned char xor_result[48] = {0};
    int tmp_len = 48;

    if (*out_len < in_len)
        return 0;

     while (enc_len < in_len) {

        // XOR against last ct or IV if first iteration
        if (enc_len == 0) {
            if (compute_xor((const char *)in + enc_len, 16,
                            (const char *)iv, 16,
                            (char *)xor_result, &tmp_len) != 1)
                return 0;
        } else {
            if (compute_xor((const char *)in + enc_len, 16,
                            (const char *)last_result, 16,
                            (char *)xor_result, &tmp_len) != 1)
                return 0;
        }

        if (encrypt_aes_128_ecb(key, xor_result, 16,
                                last_result, &tmp_len, padding) != 1)
            return 0;
        
        // copy result to buffer
        memcpy(out + enc_len, last_result, tmp_len);

        enc_len += 16;
    }

    *out_len = enc_len;
    return 1;
}

int decrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len,
                        int padding)
{
    int dec_len = 0;
    unsigned char tmp1[48] = {0};
    unsigned char tmp0[48] = {0};
    int tmp_len = 48;

    if (*out_len < in_len)
        return 0;

    while (dec_len < in_len) {

        if (decrypt_aes_128_ecb(key, in + dec_len, 16,
                                tmp1, &tmp_len, padding) != 1)
            return 0;

        // XOR against last ct or IV if first iteration
        if (dec_len == 0) {
            if (compute_xor((const char *)tmp1, 16,
                            (const char *)iv, 16,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        } else {
            if (compute_xor((const char *)tmp1, 16,
                            (const char *)tmp0, 16,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        }

        // save ct into bufer
        memcpy(tmp0, in + dec_len, 16);

        // copy result to buffer
        memcpy(out + dec_len, tmp1, 16);

        dec_len += 16;
    }

    *out_len = dec_len;
    return 1;
}