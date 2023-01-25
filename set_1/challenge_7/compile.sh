#!/bin/bash
EXECUTABLE=./aes_ecb_mode.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra -g3 main.c \
	 -I../../lib/cryptopals/inc/ -I../../lib/openssl-3.0.7/include/ \
	 -L../../lib/ -L../../lib/openssl-3.0.7/ \
     -lcryptopals_static -lcrypto \
	 -o aes_ecb_mode.out
