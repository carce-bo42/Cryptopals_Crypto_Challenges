#!/bin/bash
EXECUTABLE=./compute_xor.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra main.c -I ../../lib/cryptopals/inc/ -l cryptopals -L ../../lib/ -o compute_xor.out