#!/bin/bash
EXECUTABLE=./aes_cbc_decrypt_file.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi

gcc -Wall -Werror -Wextra -g3 -fsanitize=address main.c \
	 -I../../lib/cryptopals/inc/  \
	 -L../../lib/ \
     -lcryptopals \
	 -Wl,-rpath -Wl,../../lib/cryptopals/ \
	 -o aes_cbc_decrypt_file.out