#include "cryptopals/aes_cbc.h"
#include "cryptopals/aes_ecb.h"
#include "cryptopals/rw_ops.h"
#include "cryptopals/format_conversions.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    ECB = 0,
    CBC
} AES_MODE;

// from https://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

int encryption_oracle(const unsigned char* pt, int pt_len,
                      unsigned char* ct, int *ct_len)
{
    unsigned long seed = mix(clock(), time(NULL), getpid());
    srand(seed);
    int ret = 0;
    int aes_mode = (rand() % 2);
    unsigned char key[16] = {0};
    unsigned char iv[16] = {0};
    int before_pt_pad = (rand() % 5) + 5; // nbr between 5 and 10.
    int after_pt_pad = (rand() % 5) + 5;

    for (int i = 0; i < 16; i++) {
        key[i] = (unsigned char)(rand() % 256);
        iv[i] = (unsigned char)(rand() % 256); // Not always necessary
    }

    int _pt_len = before_pt_pad + after_pt_pad + pt_len;
    unsigned char* _pt = (unsigned char*)malloc(_pt_len * sizeof(char));
    
    if (!_pt)
        goto err;

    if (*ct_len < _pt_len) {
        goto err;
    }

    for (int i = 0; i < _pt_len; i++) {
        if (i < before_pt_pad)
            _pt[i] = (unsigned char)(rand() % 256);
        else if (i < pt_len + before_pt_pad)
            _pt[i] = pt[i - before_pt_pad];
        else
            _pt[i] = (unsigned char)(rand() % 256);
    }

    if (aes_mode == ECB) {
        printf("ECB mode\n");
        if (encrypt_aes_128_ecb(key, _pt, _pt_len, ct, ct_len, 0) != 1)
            goto err;
    } else {
        printf("CBC mode\n");
        if (encrypt_aes_128_cbc(key, iv, _pt, _pt_len, ct, ct_len, 0) != 1)
            goto err;
    }

    ret = 1;

err:

    if (_pt) { free(_pt); }

    return ret;
}



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

void free_matrix(char** mat, int size) {
    if (mat) {
        for (int i = 0; i < size; i++) {
            free(mat[i]);
        }
        free(mat);
    }
}


/* 
 * Appending from 5 to 10 bytes to each side of pt, the encryption is bounded:
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  pad  |            pt         |  pad  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * <-- 5 -->                       <-- 5 -->
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     pad     |            pt         |     pad     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * <----- 10 ---->                       <----- 10 ---->
 * 
 * Meaning that if we want to analyze ciphertexts, we must start by the
 * second block and end at the second last one. 
 * MEANING that it makes no sense to apply this method to any ciphertext of less than 
 * 4 times the blocksize (4 * 16 = 64 bytes).
 * It might work its just not statistically enough to ensure the result is correct.
 */
int main() {
    const unsigned char *pt = (const unsigned char *)
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    
    int pt_len = 224;

    unsigned char ct[512] = {0};
    int ct_len = 512;

    if (encryption_oracle(pt, pt_len, ct, &ct_len) != 1) {
        printf("encryption_oracle error\n");
    } else {
        char* ct_print = bin_to_hex((const char *)ct, ct_len);
        //write(1, ct_print, ct_len);
        //write(1, "\n", 1);
        free(ct_print);
    }

    int num_blocks = 0;
    int last_block = 0;
    char** block_array = divide_into_blocksize((const char *)(ct + 16),
                                                ct_len - 32, 16,
                                                &num_blocks,
                                                &last_block);
    // lines are 160 bytes long, this should not be happening.
    if (!block_array || last_block != 0)
        return 1;
    
    int ecb_encryption_idx = 0;
    // coincidences of pairs of bytes between the 16 byte blocks.
    for (int i = 0; i < num_blocks - 1; i++) {
        for (int k1 = 0; k1 < 15; k1++) {
        
            char byte_pair_1[2] = {block_array[i][k1],
                                    block_array[i][k1 + 1]};

            for (int j = i + 1; j < num_blocks; j++) {

                char byte_pair_2[2] = {block_array[j][k1],
                                        block_array[j][k1 + 1]};

                if (memcmp(byte_pair_1, byte_pair_2, 2) == 0)
                    ecb_encryption_idx += 1;
            }
        }
    }
    free_matrix(block_array, num_blocks);
    
    if (ecb_encryption_idx > 5) {
        printf("ECB mode\n");
    } else {
        printf("CBC mode\n");
    }

    return 0;
}