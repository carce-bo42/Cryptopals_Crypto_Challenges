#include "cryptopals/hamming_distance.h"
#include "cryptopals/format_conversions.h"
#include "cryptopals/rw_ops.h"
#include "cryptopals/repeating_key_xor.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYSIZE 80

/* 
 * Finds the <result_size> keysizes in [min_ks, max_ks - min_ks + 1)
 * whose 4 first <keysize> blocks in <buf> have the lowest
 * averaged hamming distance.
 */
int* get_keysize_from_hamming_distance(const char* buf, int buf_len,
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
    for (int keysize = min_ks; keysize < max_ks + 1; keysize++) {
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

/*
 * Out of an array with <size -1> <blocksize> bufs and one <last_blocksize> buf, 
 * returns an array of <blocksize> entries that contains the aligned chars
 * of the initial array buffs. This is,
 * ret_array[0] = blocksize_array[0][0] + blocksize_array[1][0] + ...
 * ret_array[1] = blocksize_array[0][1] + blocksize_array[1][1] + ...
 *
 * ret_array will have the following properties:
 * - It will contain <blocksize> entries.
 * - the first <last_blocksize> entries will have a size == size
 * - the rest of the entries up until <blocksize> will have size == size - 1.
 */
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


// pt_byte ^ key_byte = ct_byte
// pt_byte ^ (key_byte ^ key_byte) = ct_byte ^ key_byte
// pt_byte = ct_byte ^ key_byte
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
            free(mat[i++]);
        }
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
    
    // 3 candidates because why not
    int number_of_candidates = 3;
    int* key_sizes = get_keysize_from_hamming_distance(file_buf, file_buf_len, 
                                                       2, 80,
                                                       number_of_candidates);

    for (int i = 0; i < number_of_candidates; i++) {

        int last_blocksize = 0;
        int num_blocks = 0;

        char** blocksize_array = divide_into_blocksize(file_buf, file_buf_len,
                                                       key_sizes[i], &num_blocks,
                                                       &last_blocksize);
        if (!blocksize_array)
            return 1;

        char** sgl_char_xor_arr = construct_sgl_char_xor_arr(blocksize_array,
                                                             num_blocks,
                                                             key_sizes[i],
                                                             last_blocksize);
        if (!sgl_char_xor_arr)
            return 1;
        
        char* key = (char *)malloc(key_sizes[i]);
        for (int j = 0; j < key_sizes[i]; j++) {
            char xor_key = 0;
            if (j < last_blocksize) {
                xor_key = solve_sgl_char_xor_encryption(sgl_char_xor_arr[j],
                                                        num_blocks);
            } else {
                xor_key = solve_sgl_char_xor_encryption(sgl_char_xor_arr[j],
                                                        num_blocks - 1);
            }
            key[j] = xor_key;
        }
        
        char* decrypted_file = (char *)malloc(file_buf_len);
        if (repeating_key_xor(key, key_sizes[i], file_buf, file_buf_len,
                              decrypted_file, &file_buf_len) != 1)
        {
            return 1;
        }
        
        // This spoils the result. Not recommended.
        /*
        printf("\n\n KEY VALUE : %d\n", key_sizes[i]);
        write(1, key, key_sizes[i]);

        printf("\n\n DECRYPTED FILE\n");
        write(1, decrypted_file, (size_t)file_buf_len);
        */
        
        free(key);
        free(decrypted_file);
        free_matrix(blocksize_array, num_blocks);
        free_matrix(sgl_char_xor_arr, key_sizes[i]);
    }

    free(key_sizes);
    free(file_buf_base64); 
    return 0;
}