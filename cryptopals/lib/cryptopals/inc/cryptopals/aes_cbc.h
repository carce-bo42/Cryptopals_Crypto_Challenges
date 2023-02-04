#ifndef CRYPTOPALS_AES_CBC_H
# define CRYPTOPALS_AES_CBC_H
# pragma once

int decrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len);
                        
int encrypt_aes_128_cbc(const unsigned char* key,
                        const unsigned char* iv,
                        const unsigned char* in, int in_len,
                        unsigned char* out, int *out_len);

#endif /* CRYPTOPALS_AES_CB_CH */