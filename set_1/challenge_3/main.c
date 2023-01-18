#include "cryptopals/format_conversions.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char* ct_hex = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char ct[34] = {0};
    int ct_len = 34;
    float frequency[256] = {0.0f};

    if (hex_to_bin(ct_hex, ct, &ct_len) != 1)
        return 1;
    
    for (int i = 0; i < ct_len; i++)
        frequency[(int)ct[i]] += 1;
    for (int i = 0; i < 255; i++)
        frequency[i] /= 34;
    
    for (int i = 0; i < 255; i++) {
        if (frequency[i] != 0.0f)
            printf("frequency[%d] (%c) = %f\n", i, (char)i, frequency[i]);
    }

    // given some char c, the highest frequency corresponds to ascii '7' :
    // <some_char> ^ c = '7'
    // <some_char> ^ <some_char> ^ c = c
    // c = '7' ^ <some_char> 

    // assuming the largest frequency corresponds to the following list, from 
    // https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html.
    char xor_chars[] = "eariotnslEARIOTNSL";
    int xor_chars_len = strlen(xor_chars);

    for (int i = 0; i < xor_chars_len; i++) {
        xor_chars[i] ^= '7';
    }
    char pt[34] = {0};

    for (int j = 0; j < xor_chars_len; j++) {
        for (int i = 0; i < ct_len; i++) {
            pt[i] = 0;
            pt[i] = ct[i] ^ xor_chars[j];
        }
        printf("\n\n xor_char : %c\n", xor_chars[j]);
        write(1, pt, 34);
    }

    // The plaintext was Xored with the letter 'X'.
}