#!/bin/bash
EXECUTABLE=./hex_to_base64
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra main.c -I ../../lib/cryptopals/inc/ -l cryptopals_static -L ../../lib/ -o hex_to_base64
