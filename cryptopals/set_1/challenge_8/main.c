#include <stdio.h>
#include <stdlib.h>
#include "cryptopals/format_conversions.h"
#include <unistd.h>
#include <string.h>

int main() {

    FILE* file = NULL;
    char *line = NULL;
    size_t line_len = 0;
    
    file = fopen("file.txt", "rb");
    if (!file)
        return 1;

    ssize_t chars_read = 0;
    int sequence_nb = 0;

    int candidate_idx = 0;

    int candidate_list[260] = {0};
    char candidates[260][30] = {0};
    char candidate_most_frequent[260];

    while ((chars_read = getline(&line, &line_len, file)) != -1) {
            if (line[chars_read - 1] == '\n')
            line[chars_read - 1 ] = '\0'; // prevent from non null termination
    }

    return 0;
}