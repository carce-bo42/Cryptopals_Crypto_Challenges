#ifndef CRYPTOPALS_REPEATING_KEY_CIPHER_H
# define CRYPTOPALS_REPEATING_KEY_CIPHER_H
# pragma once

int repeating_key_xor(const char* key, int key_len,
                      const char* bin, int bin_len,
                      char* out, int *out_len);

#endif