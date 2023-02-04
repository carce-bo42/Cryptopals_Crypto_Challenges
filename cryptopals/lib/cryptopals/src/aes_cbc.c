#include "cryptopals/aes_ecb.h"
#include "cryptopals/format_conversions.h"
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
    int len_left = in_len;
    unsigned char tmp1_[16] = {0};
    unsigned char tmp0_[16] = {0};
    int tmp1_len = 16;
    int tmp0_len = 16;

    if (*out_len < in_len)
        return 0;

    do {
        // normal AES encryption
        if (encrypt_aes_128_ebc(key, in + len_left, in_len - len_left,
                                tmp1_, &tmp1_len) != 1)
            return 0;

        int aux_out_len = 16;
        if (len_left == in_len) {
            // first xor is against IV.
            if (compute_xor(tmp1_, tmp1_len, iv, 16, out, &aux_out_len) != 1)
                return 0;
        } else {
            // xor with latest ciphertext
            if (compute_xor(tmp1_, tmp1_len, tmp0_, tmp0_len,
                            out + in_len, &aux_out_len) != 1)
                return 0;
        }
        
        // save last result for next xor.
        memcpy(tmp0_, tmp1_, tmp1_len);

        // save new lengths 
        len_left -= tmp1_len;

    } while (len_left > 0);

    *out_len = in_len;
    return 1;
}