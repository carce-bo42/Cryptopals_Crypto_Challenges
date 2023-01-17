#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tools.h"

int main() {
    
    // Hex encode/decode test
    const char str[] = "hello world i am a bytearray of printable chars";
    char str_after_hex[1000];
    int str_after_hex_len = 1000;
    char* hex_rep = bin_to_hex(str, strlen(str));
    printf("hex rep : %s\n", hex_rep);
    if (hex_to_bin(hex_rep, str_after_hex, &str_after_hex_len) == 0)
        return 1;
    write(1, str_after_hex, str_after_hex_len);

    // base64 encoder test
    const char str_hex_0_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    const char str_hex_1_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d7c";
    const char str_hex_2_leftover[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6dfa3d";

    char* base_64_rep_0_leftover = hex_to_base64(str_hex_0_leftover);
    char* base_64_rep_1_leftover = hex_to_base64(str_hex_1_leftover);
    char* base_64_rep_2_leftover = hex_to_base64(str_hex_2_leftover);

    printf("\n\nencoded in base64 0 leftover : %s\n", base_64_rep_0_leftover);
    printf("\nencoded in base64 1 leftover: %s\n", base_64_rep_1_leftover);
    printf("\nencoded in base64 2 leftover: %s\n", base_64_rep_2_leftover);

    free(hex_rep);
    free(base_64_rep_0_leftover);
    free(base_64_rep_1_leftover);
    free(base_64_rep_2_leftover);

    return 0;
}