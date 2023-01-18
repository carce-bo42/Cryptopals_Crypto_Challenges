#!/bin/bash
EXECUTABLE=./compute_xor
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra main.c -I ../../lib/cryptopals/inc/ -l cryptopals_static -L ../../lib/ -o compute_xor