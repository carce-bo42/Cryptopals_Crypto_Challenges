#include "openssl/conf.h"

void init_openssl() {
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS, NULL);
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
}