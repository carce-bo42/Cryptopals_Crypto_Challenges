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

int encryption_oracle(const unsigned char* pt, int pt_len,
                      unsigned char* ct, int *ct_len)
{
    srand(time(NULL));
    int ret = 0;
    int aes_mode = (rand() % 2);
    unsigned char key[16] = {0};
    unsigned char iv[16] = {0};
    int before_pt_pad = (rand() % 5) + 5; // nbr between 5 and 10.
    int after_pt_pad = (rand() % 5) + 5;

    printf("aes_mode : %i, before_pt_pad %d, after_pt_pad %d\n", aes_mode, before_pt_pad, after_pt_pad);

    for (int i = 0; i < 16; i++) {
        key[i] = (unsigned char)(rand() % 256);
        iv[i] = (unsigned char)(rand() % 256); // Not always necessary
    }

    int _pt_len = before_pt_pad + after_pt_pad + pt_len;
    unsigned char* _pt = (unsigned char*)malloc(_pt_len * sizeof(char));
    
    if (!_pt)
        goto err;

    if (*ct_len < _pt_len) {
        write(1, "xdd?\n", 5);
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
        if (encrypt_aes_128_ecb(key, _pt, _pt_len, ct, ct_len, 1) != 1) {
            write(1, "xddd!\n", 6);
            goto err;
        }
    } else {
        if (encrypt_aes_128_cbc(key, iv, _pt, _pt_len, ct, ct_len, 1) != 1) {
            write(1, "xxddd!\n", 7);
            goto err;
        }
    }

    ret = 1;

err:

    if (_pt) { free(_pt); }

    return ret;
}


int main() {
    const unsigned char *pt = (const unsigned char *)"Hello World Good Morning";
    //const char* a = "029b4589d96082e683991911588fcae6742b25e6a41283da";
    int pt_len = 24;

    unsigned char ct[256] = {0};
    int ct_len = 256;

    if (encryption_oracle(pt, pt_len, ct, &ct_len) != 1) {
        printf("encryption_oracle error\n");
    } else {
        char* ct_print = bin_to_hex((const char *)ct, ct_len);
        write(1, ct_print, ct_len);
        write(1, "\n", 1);
    }

    return 0;
}