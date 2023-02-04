#ifndef CRYPTOPALS_AES_ECB_H
# define CRYPTOPALS_AES_ECB_H
# pragma once

int decrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len);
int encrypt_aes_128_ecb(const unsigned char* key,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len);

#endif /* CRYPTOPALS_AES_ECB_H */