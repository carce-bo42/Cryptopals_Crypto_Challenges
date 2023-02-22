#!/bin/bash
EXECUTABLE=./aes_mode_oracle.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi

gcc -Wall -Werror -Wextra -g3 main.c \
	 -I../../lib/cryptopals/inc/  \
	 -L../../lib/ \
     -lcryptopals \
	 -Wl,-rpath -Wl,../../lib/cryptopals/ \
	 -o aes_mode_oracle.out
