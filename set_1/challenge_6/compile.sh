#!/bin/bash
EXECUTABLE=./break_repeating_key_xor.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra -fsanitize=address -g3 main.c -I ../../lib/cryptopals/inc/ -l cryptopals_static -L ../../lib/ -o break_repeating_key_xor.out