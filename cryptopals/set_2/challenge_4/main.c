#include "cryptopals/aes_cbc.h"
#include "format_conversions.h"

static unsigned char key[] = {0xa3, 0x13, 0x37, 0xe0, 0xcb, 0xf2, 0xee, 0x3e,
                              0x8d, 0x1d, 0x39, 0x32, 0x4c, 0x97, 0x8e, 0x71};

int main() {

    const char* uk_b64str = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
                            "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
                            "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
                            "YnkK";
    char uk_str[256] = {0};
    int uk_len = 256;
    if (base64_to_bin(uk_b64str, uk_str, &uk_len) != 1)
        return 0;
    

    
    
}