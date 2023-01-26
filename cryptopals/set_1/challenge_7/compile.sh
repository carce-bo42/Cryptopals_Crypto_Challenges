#!/bin/bash
EXECUTABLE=./aes_ecb_mode.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi

LD_LIBRARY_PATH=/media/data2/42/cryptopals/lib:$LD_LIBRARY_PATH

gcc -Wall -Werror -Wextra -g3 main.c \
	 -I../../lib/cryptopals/inc/  \
	 -L../../lib/ \
     -lcryptopals \
	 -o aes_ecb_mode.out
