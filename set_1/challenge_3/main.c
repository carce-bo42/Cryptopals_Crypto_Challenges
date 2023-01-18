#include "cryptopals/format_conversions.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int get_max_frequency_entry(float* frequency, int size) {

    int max_frequency_entry = 0;

    for (int i = 0; i < size; i++) {
        if (frequency[i] > frequency[max_frequency_entry])
            max_frequency_entry = i;
    }
    return max_frequency_entry;
}

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

    char most_frequent = (char)get_max_frequency_entry(frequency, 255);

    // given some char c, the highest frequency corresponds to some asci k :
    // <some_char> ^ c = k
    // <some_char> ^ <some_char> ^ c = c
    // c = k ^ <some_char> 

    // assuming the largest frequency corresponds to the following list, from 
    // https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html.
    // space has been included for obvious reasons.
    char xor_chars[] = " eariotnslEARIOTNSL";
    int xor_chars_len = strlen(xor_chars);

    for (int i = 0; i < xor_chars_len; i++) {
        xor_chars[i] ^= most_frequent;
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