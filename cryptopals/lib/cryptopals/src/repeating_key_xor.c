
int repeating_key_xor(const char* key, int key_len,
                      const char* bin, int bin_len,
                      char* out, int *out_len)
{
    if (*out_len < bin_len || key_len == 0)
        return 0;

    for (int i = 0, j = 0; i < bin_len; i++, j++) {
        j = (j == key_len) ? 0 : j;
        out[i] = bin[i] ^ key[j];
    }

    *out_len = bin_len;
    return 1;
}