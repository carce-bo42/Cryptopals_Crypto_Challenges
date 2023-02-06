#!/bin/bash
EXECUTABLE=./pkcs7_padding.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi

gcc -Wall -Werror -Wextra -g3 -fno-stack-protector -fsanitize=address main.c \
	 -I../../lib/cryptopals/inc/  \
	 -L../../lib/ \
     -lcryptopals \
	 -Wl,-rpath -Wl,../../lib/cryptopals/ \
	 -o pkcs7_padding.out
