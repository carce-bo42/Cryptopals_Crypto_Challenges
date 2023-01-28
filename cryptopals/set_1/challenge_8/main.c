#include <stdio.h>
#include <stdlib.h>
#include "cryptopals/format_conversions.h"
#include "cryptopals/hamming_distance.h"
#include "cryptopals/matrix_ops.h"
#include "cryptopals/repeating_key_xor.h"
#include <unistd.h>
#include <string.h>

char** divide_into_blocksize(const char* buf, int len,
                             int blocksize,
                             int *size,
                             int* last_blocksize)
{
    if (!buf || len < blocksize || !last_blocksize) {
        return NULL;
    }
    
    // get how many blocksizes can we create from buf.
    *last_blocksize = len % blocksize;
    int fixed_size = (len / blocksize);
    int var_size = *last_blocksize == 0 ? 0 : 1;

    char** blocks = (char **)malloc(sizeof(char*) * (fixed_size + var_size));
    
    for (int i = 0; i < fixed_size; i++)
        blocks[i] = (char *)malloc(blocksize);
    
    if (*last_blocksize)
        blocks[fixed_size] = (char *)malloc((*last_blocksize));

    for (int i = 0; i < fixed_size; i++) {
        memcpy(blocks[i], buf + (i * blocksize), blocksize);
    }

    if (*last_blocksize != 0)
        memcpy(blocks[fixed_size], buf + (fixed_size * blocksize),
                                   *last_blocksize);
    *size = fixed_size + var_size;
    return blocks;
}

char** construct_sgl_char_xor_arr(char** blocksize_array, int size,
                                  int blocksize, int last_blocksize)
{
    // The array has as many entries as bytes the key has, this is,
    // the same as blocksize.
    char** sgl_char_xor_arr = (char **)malloc(sizeof(char*) * blocksize);

    // The first <last_blocksize> entries will have a full size,
    // the others will have one less byte.
    int k = 0;
    while (k < last_blocksize)
        sgl_char_xor_arr[k++] = (char *)malloc(size);

    while (k < blocksize)
        sgl_char_xor_arr[k++] = (char *)malloc(size - 1);

    for (int i = 0; i < blocksize; i++) {
        
        // transpose blocksize_array. i is fixed here.
        for (int j = 0; j < size - 1; j++) {
            sgl_char_xor_arr[i][j] = blocksize_array[j][i];
        }
        
        // write one more byte for the first <last_blocksize> iterations.
        if (last_blocksize != 0) {
            sgl_char_xor_arr[i][size - 1] = blocksize_array[size - 1][i];
            --last_blocksize;
        }
    }
    return sgl_char_xor_arr;
}

char solve_sgl_char_xor_encryption(const char* buf, int len) {

    char eng_most_freq[] = " eariotnslEARIOTNSL";
    int xor_chars_len = strlen(eng_most_freq);

    char* pt = (char *)malloc(len);

    double english_likelyhood[255] = {0};

    for (int i = 0; i < 255; i++) {

        for (int j = 0; j < len; j++) {
            pt[j] = buf[j] ^ (char)i;
        }

        for (int j = 0; j < len; j++) {
            for (int k = 0; k < xor_chars_len; k++) {
                if (pt[j] == eng_most_freq[k]) {
                    english_likelyhood[i] += 1.0;
                    break;
                }
            }
        }
        english_likelyhood[i] /= (double)len;
    }
    free(pt);
    return (char)get_max_val_entry(english_likelyhood, 255);
}

void free_matrix(char** mat, int size) {
    if (mat) {
        for (int i = 0; i < size; i++) {
            free(mat[i]);
        }
        free(mat);
    }
}

int main() {

    FILE* file = NULL;
    char *line = NULL;
    size_t line_len = 0;
    
    file = fopen("file.txt", "rb");
    if (!file)
        return 1;

    ssize_t chars_read = 0;

    double ecb_encryption_idx[1024] = {0.0};
    int line_count = 0;

    while ((chars_read = getline(&line, &line_len, file)) != -1) {

        if (line[chars_read - 1] == '\n')
            line[chars_read - 1 ] = '\0';

        char bin[200] = {0};
        int bin_len = 200;

        if (hex_to_bin(line, bin, &bin_len) != 1) {
            if (line)
                free(line);
            return 1;
        }

        int num_blocks = 0;
        int last_block = 0;
        char** block_array = divide_into_blocksize(bin, bin_len, 16,
                                                    &num_blocks,
                                                    &last_block);
        // lines are 160 bytes long, this should not be happening.
        if (!block_array || last_block != 0)
            return 1;
        
        // coincidences of pairs of bytes between the 16 byte blocks.
        for (int i = 0; i < num_blocks - 1; i++) {
            for (int k1 = 0; k1 < 15; k1++) {
            
                char byte_pair_1[2] = {block_array[i][k1],
                                       block_array[i][k1 + 1]};

                for (int j = i + 1; j < num_blocks; j++) {

                    char byte_pair_2[2] = {block_array[j][k1],
                                           block_array[j][k1 + 1]};

                        if (memcmp(byte_pair_1, byte_pair_2, 2) == 0)
                            ecb_encryption_idx[line_count] += 1;
                }
            }
        }

        line_count++;
        free_matrix(block_array, num_blocks);
    }
    free(line);
    line = NULL;

    int line_size = 320;

    for (int iter = 0; iter < 1; iter++) {
        int line_number = get_max_val_entry(ecb_encryption_idx, line_count);
        ecb_encryption_idx[line_number] = 0.0;

        // skip the extra newline char
        fseek(file, ((int)line_size + 1) * line_number, SEEK_SET);
        chars_read = getline(&line, &line_len, file);
        line[chars_read - 1 ] = '\0';

        char ecb_enc[160] = {0};
        int ecb_enc_len = 160;

        if (hex_to_bin(line, ecb_enc, &ecb_enc_len) != 1) {
            return 1;
        }

        int num_blocks = 0;
        int last_block = 0;
        char** block_array = divide_into_blocksize(ecb_enc, ecb_enc_len, 16,
                                                    &num_blocks,
                                                    &last_block);
        if (!block_array)
            return 1;

        char** transposed = construct_sgl_char_xor_arr(block_array,
                                                        num_blocks, 16,
                                                        last_block);
        if (!transposed)
            return 1;

        char key[16] = {0};
        for (int j = 0; j < 16; j++) {
            char xor_key = 0;
            if (j < last_block && last_block == 0) {
                xor_key = solve_sgl_char_xor_encryption(transposed[j],
                                                        num_blocks);
            } else {
                xor_key = solve_sgl_char_xor_encryption(transposed[j],
                                                        num_blocks - 1);
            }
            key[j] = xor_key;
        }
        char decrypted_line[160] = {0};
        if (repeating_key_xor(key, 16, ecb_enc, ecb_enc_len,
                                decrypted_line, &ecb_enc_len) != 1)
        {
            return 1;
        }

        printf("KEY VALUE : %d\n", 16);
        write(1, key, 16);

        printf("\nDECRYPTED FILE\n");
        write(1, decrypted_line, (size_t)ecb_enc_len);

        free_matrix(block_array, num_blocks);
        free_matrix(transposed, 16);
    }
    free(line);

    return 0;
}