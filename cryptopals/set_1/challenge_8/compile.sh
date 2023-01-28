#!/bin/bash
EXECUTABLE=./aes_ecb_detect.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi

gcc -Wall -Werror -Wextra -g3 -fsanitize=address main.c \
	 -I../../lib/cryptopals/inc/  \
	 -L../../lib/ \
     -lcryptopals \
	 -Wl,-rpath -Wl,../../lib/cryptopals/ \
	 -o aes_ecb_detect.out
