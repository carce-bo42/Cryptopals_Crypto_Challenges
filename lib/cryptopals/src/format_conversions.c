#include <stdlib.h>
#include <string.h>

/* 
 * Hex representation produces 1 byte for each 4 bits of initial sequence.
 * Or, from another perspective, 2 output bytes for each input byte.
 * Thus, a hex sequence corresponding to an n byte sequence has n * 2 bytes.
 * 
 * Base64 representation produces 1 byte for each 6 bits of initial sequence.
 * Which means 4 output bytes for each 3 input bytes.
 * Thus, a base64 sequence corresponding to an n byte sequence has
 * ( n / 3 ) * 4 bytes of base64 representation + ( n % 3 ) padding bytes
 * represented by a '='. 
 *
 * 
 * Useful hex values:
 *
 * 1111 1100 = 0xfc
 * 0000 0011 = 0x03
 * 1111 0000 = 0xf0
 * 0000 1111 = 0x0f
 * 1100 0000 = 0xc0
 * 0011 1111 = 0x3f
 *
 */


/*
 * PS: Hex and base64 representations are assumed to be null terminated strings,
 * hence they do not need a length argument.
 */

static const char base_64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base_hex[16] = "0123456789abcdef";

int hex_to_bin(const char* hex, char* bin, int *bin_len) {

    int hex_len = 0;
    int shift = 4;
    int pos = 0;

    if (!hex || !bin || !bin_len)
        return 0;

    hex_len = (int)strlen(hex);

    if (*bin_len < hex_len / 2
        || hex_len % 2) // corrupted hex sequence
    {
        return 0; 
    }

    for (int i = 0; hex[i]; ++i) {
        
        // no good reason to think <bin> is set to zeros.
        if (shift == 4)
            bin[pos] = 0;

        if (hex[i] >= '0' && hex[i] <= '9')
            bin[pos] += (hex[i] - '0') << shift;
        else if (hex[i] >= 'A' && hex[i] <= 'Z')
            bin[pos] += (hex[i] - 'A' + 10) << shift;
        else if (hex[i] >= 'a' && hex[i] <= 'z')
            bin[pos] += (hex[i] - 'a' + 10) << shift;
        else
            return 0; // non hex char found

        if (i % 2) {
            shift = 4;
            ++pos;
        } else {
            shift = 0;
        }
    }
    *bin_len = hex_len / 2;
    return 1;
}

// to test last function.
char* bin_to_hex(const char* bin, int bin_len) {

    char* hex = NULL;
    int pos = 0;

    if (!bin
        || bin_len <= 0
        || (hex = (char *)malloc((bin_len * 2) + 1)) == NULL)
    {
        return 0;
    }

    for (int i = 0; i < bin_len; i++) {
        hex[pos++] = base_hex[(bin[i] & 0xf0) >> 4];
        hex[pos++] = base_hex[bin[i] & 0x0f];
    }

    hex[pos] = '\0';
    return hex;
}

int get_base64_len(int bin_len) {

    int groups_of_three_bytes = bin_len / 3;
    int leftover_bytes = (bin_len) % 3;

    if (leftover_bytes == 0)
        return groups_of_three_bytes * 4;
    else
        return (groups_of_three_bytes + 1) * 4;
}


/*
 * If bin_len % 3 == 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |8 leftover bits|      0's      |      0's      |  -> bin
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |b64 byte 1 |b64 byte 2 |  PADDING  |  PADDING  |  -> b64
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * If bin_len % 3 == 2
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |8 leftover bits|8 leftover bits|      0's      |  -> bin
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |b64 byte 1 |b64 byte 2 |b64 byte 3 |  PADDING  |  -> b64
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
char* bin_to_base64(const char* bin, int bin_len) {

    int b64_len = 0;
    char *b64 = NULL;
    int pos = 0;
    int pos64 = 0;

    if (!bin
        || bin_len <= 0
        || (b64_len = get_base64_len(bin_len)) <= 0
        || (b64 = (char *)malloc(b64_len + 1)) == NULL)
    {
        return NULL;
    }

    while (pos < bin_len) {

        int iteration = pos64 % 4;
        char val_pos = bin[pos];
        char val_next_pos = pos + 1 < bin_len ? bin[pos + 1] : 0;

        // b64[pos64] : = 6 msb of val_pos
        if (iteration == 0) {
            b64[pos64] = base_64[(val_pos & 0xfc) >> 2];
        // b64 := 2 lsb of val_pos | 4 msb of val_next_pos
        } else if (iteration == 1) {
            b64[pos64] = base_64[((val_pos & 0x03) << 4)
                                 + ((val_next_pos & 0xf0) >> 4)];
            ++pos;
        // b64 := 4 lsb of val_pos | 2 msb of val_next_pos
        } else if (iteration == 2) {
            b64[pos64] = base_64[((val_pos & 0x0f) << 2)
                                 + ((val_next_pos & 0xc0) >> 6)];
            ++pos;
        // b64 := 6 lsb of val_pos
        } else {
            b64[pos64] = base_64[(int)(val_pos & 0x3f)];
            ++pos;
        }
        ++pos64;
    }

    while (pos64 < b64_len)
        b64[pos64++] = '=';
    
    b64[pos64] = '\0';
    return b64;
}

int base64_to_bin(const char* base64, char* bin, int *bin_len) {

    int base64_len = 0;
    int result_len = 0;
    int padding_bytes = 0;
    int pos = 0;

    if (!bin || base64 == 0 || base64_len % 4) // corrupted base64 sequence
        return 0;

    base64_len = (int)strlen(base64);
    result_len = (base64_len * 3) / 4;
    padding_bytes += (base64[base64_len - 1] == '=') ? 1 : 0;
    padding_bytes += (base64[base64_len - 2] == '=') ? 1 : 0;
    result_len -= padding_bytes;
    base64_len -= padding_bytes;

    if (*bin_len < result_len)
        return 0;

    for (int i = 0; i < base64_len; i++) {

        int iteration = i % 4;
        char value_6bit = 0;

        if (base64[i] >= 'A' && base64[i] <= 'Z')
            value_6bit = base64[i] - 'A';
        else if (base64[i] >= 'a' && base64[i] <= 'z')
            value_6bit = (base64[i] - 'a') + 26;
        else if (base64[i] >= '0' && base64[i] <= '9')
            value_6bit = (base64[i] - '0') + 52;
        else if (base64[i] == '+')
            value_6bit = 62;
        else if (base64[i] == '/')
            value_6bit = 63;
        else
            return 0;

        if (iteration == 0) {
            bin[pos] = 0;
            bin[pos] += value_6bit << 2;
        } else if (iteration == 1) {
            bin[pos] += value_6bit >> 4;
            ++pos;
            bin[pos] = 0;
            bin[pos] += (value_6bit & 0x0f) << 4;
        } else if (iteration == 2) {
            bin[pos] += value_6bit >> 2;
            ++pos;
            bin[pos] = 0;
            bin[pos] += (value_6bit & 0x03) << 6;
        } else {
            bin[pos] += (value_6bit);
            ++pos;
        }
    }

    *bin_len = result_len;
    return 1;
}