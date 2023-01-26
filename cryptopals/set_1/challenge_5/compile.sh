#!/bin/bash
EXECUTABLE=./implement_repeated_key.out
if [ -f "$FILE" ]; then
	rm -rf $FILE
fi
gcc -Wall -Werror -Wextra -fsanitize=address -g3 main.c -I ../../lib/cryptopals/inc/ -l cryptopals -L ../../lib/ -o implement_repeated_key.out