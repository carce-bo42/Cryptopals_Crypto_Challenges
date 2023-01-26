#include "cryptopals/format_conversions.h"
#include "cryptopals/repeating_key_xor.h"
#include <stdio.h>
#include <stdlib.h>


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