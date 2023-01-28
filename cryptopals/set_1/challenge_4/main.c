#include <stdio.h>
#include <stdlib.h>
#include "cryptopals/format_conversions.h"
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

    FILE* file = NULL;
    char *line = NULL;
    size_t line_len = 0;
    
    file = fopen("file.txt", "rb");
    if (!file)
        return 1;

    ssize_t chars_read = 0;
    int sequence_nb = 0;

    int candidate_idx = 0;

    int candidate_list[260] = {0};
    char candidates[260][30] = {0};
    char candidate_most_frequent[260];

    while ((chars_read = getline(&line, &line_len, file)) != -1)  {
        if (line[chars_read - 1] == '\n')
            line[chars_read - 1 ] = '\0'; // prevent from non null termination

        char bin[30] = {0};
        int bin_len = 30;

        if (hex_to_bin(line, bin, &bin_len) != 1) {
            if (line)
                free(line);
            return 1;
        }

        float hamming_dst[256] = {0.0f};
        int keysize = 16;

        double hd_1_2 = hamming_distance(line, keysize,
                                         line + keysize, keysize)
                                / (double)keysize;
        double hd_1_3 = hamming_distance(line, keysize,
                                         line + (2 * keysize), keysize)
                                / (double)keysize;
        double hd_1_4 = hamming_distance(line, keysize,
                                         line + (3 * keysize), keysize)
                                / (double)keysize;
        double hd_2_3 = hamming_distance(line + keysize, keysize,
                                         line + (2 * keysize), keysize)
                                / (double)keysize;
        double hd_2_4 = hamming_distance(line + keysize, keysize,
                                         line + (3 * keysize), keysize)
                                / (double)keysize;
        double hd_3_4 = hamming_distance(line + (2 * keysize), keysize,
                                         line + (3 * keysize), keysize)
                                / (double)keysize;
        hd_keysizes[keysize - min_ks] = (hd_1_2 + hd_1_3 + hd_1_4
                                        + hd_2_3 + hd_2_4 + hd_3_4)
                                        / 6.0f;

        /* I could do math, but I could also be heuristic. The well-known birthday
         * paradox, states that for 57 random people to be asked for their birthday,
         * there is a 99.666% probability of coincidence in at least 1.
         * So, in 53 different chars (alphabet caps/lowcase + space), this reduces to
         * 8.276... which means with 9 chars the probability that no repetition happened
         * is almost negligible. If random, obviously.
         * first im going to print how many times different bits are happening,
         * see what happens:
         */

        //printf("different bits for sequence %d : %d\n", sequence_nb, different_bits);

        if (different_bits < 25) {
            candidate_list[candidate_idx] = sequence_nb;
            memcpy(candidates[candidate_idx], bin, bin_len);
            candidate_most_frequent[candidate_idx] = 
                (unsigned char)get_max_frequency_entry(frequency, 255);
            ++candidate_idx;
        }
        ++sequence_nb;
    }

    int candidate_size = candidate_idx;
    printf("number of candidates : %d\n\n", candidate_size);

    for (int c_idx = 0; c_idx < candidate_size; c_idx++) {

        printf("\n\n------ CANDIDATE IDX : %d ------\n", candidate_list[c_idx]);
        //char xor_chars[] = " eariotnslEARIOTNSL";
        char xor_chars[] = " ";
        int xor_chars_len = strlen(xor_chars);

        for (int i = 0; i < xor_chars_len; i++) {
            xor_chars[i] ^= candidate_most_frequent[c_idx];
        }
        char pt[30] = {0};

        for (int j = 0; j < xor_chars_len; j++) {
            for (int i = 0; i < 30; i++) {
                pt[i] = 0;
                pt[i] = candidates[c_idx][i] ^ xor_chars[j];
            }
            printf("\nxor_char : %c\n", xor_chars[j]);
            write(1, pt, 30);
        }
    }

    if (line)
        free(line);
    fclose(file);
    return 0;
}