#include "format_conversions.h"
#include <stdio.h>
#include <stdlib.h>

int compute_xor(const char* lhs, int lhs_size,
                const char* rhs, int rhs_size,
                char* result, int* result_len)
{
    if (lhs_size != rhs_size)
        return 0;
    
    if (lhs_size > *result_len)
        return 0;
    
    for(int i = 0; i < lhs_size; i++)
        result[i] = lhs[i] ^ rhs[i];
    
    *result_len = lhs_size;
    return 1;
}

int main() {

    const char lhs[] = "1c0111001f010100061a024b53535009181c";
    const char rhs[] = "686974207468652062756c6c277320657965";

    printf("xor between %s and %s :\n", lhs, rhs);

    char lhs_bin[18] = {0};
    int lhs_bin_len = 18;
    char rhs_bin[18] = {0};
    int rhs_bin_len = 18;

    if (hex_to_bin(lhs, lhs_bin, &lhs_bin_len) != 1
        || hex_to_bin(rhs, rhs_bin, &rhs_bin_len) != 1)
    {
        return 0;
    }

    char result[18] = {0};
    int result_len = 18;

    if (compute_xor(lhs_bin, lhs_bin_len,
                    rhs_bin, rhs_bin_len,
                    result, &result_len) != 1)
    {
        return 0;
    }

    char* printable_result = bin_to_hex(result, result_len);

    printf("result : %s\n", printable_result);

    free(printable_result);
    return 0;
}