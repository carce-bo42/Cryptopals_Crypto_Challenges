#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tools.h"



char* hex_to_base64(const char* hex) {

    int bin_len = 0;
    char* bin = NULL;
    char* b64_ret = NULL;

    if (!hex)
        return NULL;
    
    bin_len = 2 * (int)strlen(hex);
    bin = (char *)malloc(bin_len);

    if (!bin)
        return NULL;
    
    if (1 != hex_to_bin(hex, bin, &bin_len)) {
        free(bin);
        return NULL;
    }

    b64_ret = bin_to_base64(bin, bin_len);
    
    free(bin);
    return b64_ret;
}

int main() {
    
    const char str[] = "hello world i am a bytearray of printable chars";

    // Hex encode/decode test
    char str_after_hex[1000];
    int str_after_hex_len = 1000;
    char* hex_rep = bin_to_hex(str, strlen(str));
    printf("hex rep : %s\n", hex_rep);
    if (hex_to_bin(hex_rep, str_after_hex, &str_after_hex_len) == 0)
        return 1;
    write(1, str_after_hex, str_after_hex_len);
    printf("\n");

    free(hex_rep);

    // base64 encode/decode test
    char str_after_b64[1000];
    int str_after_b64_len = 1000;
    char* b64_rep = bin_to_base64(str, strlen(str));
    printf("b64 rep : %s\n", b64_rep);
    if (base64_to_bin(b64_rep, str_after_b64, &str_after_b64_len) == 0)
        return 1;
    write(1, str_after_b64, str_after_b64_len);
    printf("\n");

    free(b64_rep);

    // The actual challenge exercise
    const char str_hex_0_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    const char str_hex_1_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d7c";
    const char str_hex_2_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6dfa3d";

    char* base_64_rep_0_leftover = hex_to_base64(str_hex_0_leftover);
    char* base_64_rep_1_leftover = hex_to_base64(str_hex_1_leftover);
    char* base_64_rep_2_leftover = hex_to_base64(str_hex_2_leftover);

    printf("\n\nencoded in base64 0 leftover : %s\n", base_64_rep_0_leftover);
    printf("\nencoded in base64 1 leftover: %s\n", base_64_rep_1_leftover);
    printf("\nencoded in base64 2 leftover: %s\n", base_64_rep_2_leftover);

    free(base_64_rep_0_leftover);
    free(base_64_rep_1_leftover);
    free(base_64_rep_2_leftover);

    return 0;
}