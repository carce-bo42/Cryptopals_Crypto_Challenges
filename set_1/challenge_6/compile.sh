#!/bin/bash
EXECUTABLE=./break_repeating_key_xor.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra -g3 main.c -I ../../lib/cryptopals/inc/ 
    -lcryptopals_static -L ../../lib/ -o break_repeating_key_xor.out