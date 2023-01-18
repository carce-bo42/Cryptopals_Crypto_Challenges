#include "cryptopals/format_conversions.h"

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