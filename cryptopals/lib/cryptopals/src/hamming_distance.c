

/*
 * XOR between same bits is 0.
 * XOR between different bits is 1.
 * 
 * To compute hamming distance between 2 bytes,
 * >> XOR them
 * >> count bits set to 1.
 */
static int byte_hamming_distance(char b1, char b2) {
    
    char xor_res = b1 ^ b2;
    int distance = 0;

    for (int i = 0; i < 8; i++)
        distance += (xor_res & (0x01 << i)) != 0 ? 1 : 0;
    
    return distance;
}

int hamming_distance(const char* s1, int s1_len,
                     const char* s2, int s2_len)
{
    int distance = 0;

    if (s1_len != s2_len) {
        return 0;
    }

    for (int i = 0; i < s1_len; i++)
        distance += byte_hamming_distance(s1[i], s2[i]);

    return distance;
}