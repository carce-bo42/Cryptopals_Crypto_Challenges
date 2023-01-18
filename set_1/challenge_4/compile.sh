#!/bin/bash
EXECUTABLE=./find_single_char_xor_encrypted.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra -fsanitize=address -g3 main.c -I ../../lib/cryptopals/inc/ -l cryptopals_static -L ../../lib/ -o find_single_char_xor_encrypted.out
