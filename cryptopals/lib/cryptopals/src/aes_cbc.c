#include "cryptopals/aes_ecb.h"
#include "cryptopals/logic_gates.h"
#include <string.h>

/*
 * Much in the same way as the signature of AES-128-ECB methods, 
 * An ivlen parameter is not needed since the the ivlen is fixed.
 * The iv length MUST be 
 */

int encrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len)
{
    int enc_len = 0;
    unsigned char tmp1[16] = {0};
    unsigned char tmp0[16] = {0};
    int tmp_len = 16;

    if (*out_len < in_len)
        return 0;

     while (enc_len < in_len) {
        // get pt into bufer
        memcpy(tmp1, in + enc_len, 16);

        // XOR against last ct or IV if first iteration
        if (enc_len == 0) {
            if (compute_xor((const char *)tmp1, tmp_len,
                            (const char *)iv, 16,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        } else {
            if (compute_xor((const char *)tmp1, tmp_len,
                            (const char *)tmp0, tmp_len,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        }

        if (encrypt_aes_128_ecb(key, tmp1, tmp_len,
                                tmp0, &tmp_len) != 1)
            return 0;

        // save last result for next XOR.
        memcpy(tmp0, tmp1, tmp_len);

        // copy result to buffer
        memcpy(out + enc_len, tmp1, tmp_len);

        enc_len += tmp_len;
    }

    *out_len = in_len;
    return 1;
}


int decrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len)
{
    int dec_len = 0;
    unsigned char tmp1[16] = {0};
    unsigned char tmp0[16] = {0};
    int tmp_len = 16;

    if (*out_len < in_len)
        return 0;

    while (dec_len < in_len) {

        if (decrypt_aes_128_ecb(key, in + dec_len, 16,
                                tmp1, &tmp_len) != 1)
            return 0;

        // XOR against last ct or IV if first iteration
        if (dec_len == 0) {
            if (compute_xor((const char *)tmp1, tmp_len,
                            (const char *)iv, 16,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        } else {
            if (compute_xor((const char *)tmp1, tmp_len,
                            (const char *)tmp0, tmp_len,
                            (char *)tmp1, &tmp_len) != 1)
                return 0;
        }

        // save ct into bufer
        memcpy(tmp0, in + dec_len, 16);

        // copy result to buffer
        memcpy(out + dec_len, tmp1, tmp_len);

        dec_len += tmp_len;

    }

    *out_len = dec_len;
    return 1;
}