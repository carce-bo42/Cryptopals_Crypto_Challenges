#include <string.h>
#include <stdio.h>
#include "cryptopals/format_conversions.h"
#include <stdlib.h>

int get_pkcs7_padded_buffer(char* in, int in_len, char* out, int *out_len,
                            int final_len)
{
    if (in_len > final_len
        || *out_len < final_len)
        return 0;

    int pad_byte = final_len - in_len;

    if (pad_byte == 0) // pkcs7 specific.
        pad_byte = 0x10;
    
    memcpy(out, in, in_len);
    memset(out + in_len, pad_byte, (size_t)pad_byte);

    *out_len = final_len;
    return 1;
}


int main() {

    char* str = "YELLOW SUBMARINE";
    char out[100] = {0};
    int outlen = 100;

    if (1 != get_pkcs7_padded_buffer(str, strlen(str), out, &outlen, 20)) {
        return 1;
    }

    char* hex_result = bin_to_hex(out, outlen);

    printf("%s\n", hex_result);
    free(hex_result);
    return 0;
}