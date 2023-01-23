#include "cryptopals/hamming_distance.h"
#include "cryptopals/format_conversions.h"
#include "cryptopals/rw_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYSIZE 80

int get_min_val_entry(double* mat, int size) {

    int min_val_entry = 0;

    for (int i = 0; i < size; i++) {
        if (mat[i] < mat[min_val_entry])
            min_val_entry = i;
    }
    return min_val_entry;
}

/* 
 * Finds the <result_size> keysizes in [min_ks, max_ks - min_ks + 1)
 * whose 4 first <keysize> blocks in <buf> have the lowest
 * averaged hamming distance.
 */
int* get_keysize_from_humming_distance(const char* buf, int buf_len,
                                       int min_ks, int max_ks,
                                       int result_size)
{
    if (result_size <= 0
        || 2 * buf_len < max_ks
        || !(max_ks > min_ks)
        || result_size > (max_ks - min_ks +1))
    {
        return NULL;
    }

    int keysize_test_space = max_ks - min_ks + 1;
    double *hd_keysizes = (double *)malloc(sizeof(double) * keysize_test_space);
    if (!hd_keysizes)
        return NULL;
    
    // Average hamming distance between 4 first KEYSIZE blocks.
    for (int keysize = min_ks; keysize < max_ks; keysize++) {
        double hd_1_2 = hamming_distance(buf, keysize,
                                         buf + keysize, keysize)
                                / (double)keysize;
        double hd_1_3 = hamming_distance(buf, keysize,
                                         buf + (2 * keysize), keysize)
                                / (double)keysize;
        double hd_1_4 = hamming_distance(buf, keysize,
                                         buf + (3 * keysize), keysize)
                                / (double)keysize;
        double hd_2_3 = hamming_distance(buf + keysize, keysize,
                                         buf + (2 * keysize), keysize)
                                / (double)keysize;
        double hd_2_4 = hamming_distance(buf + keysize, keysize,
                                         buf + (3 * keysize), keysize)
                                / (double)keysize;
        double hd_3_4 = hamming_distance(buf + (2 * keysize), keysize,
                                         buf + (3 * keysize), keysize)
                                / (double)keysize;
        hd_keysizes[keysize - min_ks] = (hd_1_2 + hd_1_3 + hd_1_4
                                        + hd_2_3 + hd_2_4 + hd_3_4)
                                        / 6.0f;
    }

    int *keysizes = (int *)malloc((sizeof(int) * result_size));
    for (int i = 0; i < result_size; i++) {
        keysizes[i] = get_min_val_entry(hd_keysizes, keysize_test_space);
        
        // set to a high value to get next minimum.
        hd_keysizes[keysizes[i]] = 4.0f; 

        // keysize n is at n - min_ks entry.
        keysizes[i] += min_ks;
    }

    free(hd_keysizes);
    return keysizes;
}

/*
 * returns a NULL terminated array of buffers of size blocksize
 * The last entry size can be <= blocksize and its size is written
 * in last_blocksize.
 */ 
char** divide_into_blocksize(const char* buf, int len,
                             int blocksize,
                             int *size,
                             int* last_blocksize)
{
    if (!buf || len > blocksize || !last_blocksize) {
        return NULL;
    }
    
    // get how many blocksizes can we create from buf.
    *last_blocksize = len % blocksize;
    int fixed_size = (len / blocksize);
    int var_size = *last_blocksize == 0 ? 0 : 1;

    char** blocks = (char **)malloc((fixed_size + var_size));
    for (int i = 0; i < fixed_size; i++)
        blocks[i] = (char *)malloc(blocksize);
    if (*last_blocksize)
        blocks[fixed_size] = (char *)malloc((*last_blocksize));

    for (int i = 0; i < fixed_size; i++)
        memcpy(&blocks[i][0], buf + (i * blocksize), blocksize);
    if (*last_blocksize != 0)
        memcpy(&blocks[fixed_size][0], buf + (fixed_size * blocksize),
                                    *last_blocksize);
    *size = fixed_size + var_size;
    return blocks;
}

/*
 * Out of an array with <size -1> <blocksize> bufs and one <last_blocksize> buf, 
 * returns an array of <blocksize> entries that contains the aligned chars
 * of the initial array buffs. This is,
 * ret_array[0] = blocksize_array[0][0] + blocksize_array[1][0] + ...
 * ret_array[1] = blocksize_array[0][1] + blocksize_array[1][1] + ...
 *
 * <full_size_entries> returns how many of the entries in ret_array have
 * a size equal to <size>, starting at ret_array[0]. The rest of the entries
 * will have a length of <size> - 1, logically.
 */
int construct_sgl_char_xor_arr(char** blocksize_array, int size,
                               int blocksize, int last_blocksize,
                               int *full_size_entries)
{
    char** sgl_char_xor_arr = (char **)malloc(blocksize);

    // allocate <size> chars for all buffers within last_block_size range.
    int total_chars = blocksize;
    while (last_blocksize--)
        sgl_char_xor_arr = (char *)malloc(size);

    total_chars -= blocksize;
    while (total_chars--)
        sgl_char_xor_arr = (char *)malloc(size - 1);

    for (int j = 0; j < blocksize; j++) {
        int k = 0;
        for (int i = 0; i < size - 1; i++) {
            sgl_char_xor_arr[j][k] = blocksize_array[i][j];
            k++;
        }
        // copy last byte in case it exists.
        if (last_blocksize != 0) {
            sgl_char_xor_arr[j][k] = blocksize_array[size - 1][j];
        }
    }
}


void free_matrix(char** mat) {
    if (mat) {
        int i = 0;
        while (mat[i] != NULL) {
            free(mat[i]);
        }
        free(mat[i]);
        free(mat);
    }
}

int main() {

    if (hamming_distance("this is a test", 14, "wokka wokka!!!", 14) != 37)
        return 0;

    char* file_buf_base64 = get_string_from_file("enc_file.txt", 0);
    char file_buf[4096] = {0};
    int file_buf_len = 4096;

    if (base64_to_bin(file_buf_base64, file_buf, &file_buf_len) == 0)
        return 1;
    
    int number_of_candidates = 3;
    int* key_sizes = get_keysize_from_humming_distance(file_buf, file_buf_len, 
                                                       2, 80,
                                                       number_of_candidates);
    // keysizes : 29, 58, 5.
    for (int i = 0; i < number_of_candidates; i++) {
        int last_blocksize = 0;
        int num_blocks = 0;
        char** blocksize_array = divide_into_blocksize(file_buf, file_buf_len,
                                                key_sizes[i], &num_blocks,
                                                &last_blocksize);
        char** sgl_char_xor_arr = construct_sgl_char_xor_arr(blocksize_array,
                                                             key_sizes[i],
                                                             last_blocksize);
        free_matrix(blocksize_array);
    }


    

    free(key_sizes);
    free(file_buf_base64); 
    return 0;
}