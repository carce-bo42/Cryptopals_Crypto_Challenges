#include "cryptopals/format_conversions.h"
#include "cryptopals/logic_gates.h"
#include <stdio.h>
#include <stdlib.h>

int repeating_key_xor(const char* key, int key_len,
                      const char* bin, int bin_len,
                      char* out, int *out_len)
{
    if (*out_len < bin_len || key_len == 0)
        return 0;

    for (int i = 0, j = 0; i < bin_len; i++, j++) {
        j = (j == key_len) ? 0 : j;
        out[i] = bin[i] ^ key[j];
    }

    *out_len = bin_len;
    return 1;
}

int main() {
    
    const char *pt = "Burning 'em, if you ain't quick and nimble"
                     "\nI go crazy when I hear a cymbal";
    int pt_len = 74;

    char ct[100] = {0};
    int ct_len = 100; 

    if (repeating_key_xor("ICE", 3, pt, pt_len, ct, &ct_len) != 1)
        return 1;

    char* hex_ct = bin_to_hex(ct, ct_len);
    printf("hex_ct : %s\n", hex_ct);

    free(hex_ct);
    return 0;
}