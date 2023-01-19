#include "cryptopals/hamming_distance.h"
#include <stdio.h>


// this one returns a null terminated string.
char* get_buffer_from_file(const char* file_name, int *buf_len) {

    char *file_buf = NULL;
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
        return NULL;
    
    // Go to the end of the file.
    if (fseek(fp, 0L, SEEK_END) == 0) {

        // Get the size of the file.
        long buf_sz = ftell(fp);
        if (buf_sz != -1)
            return NULL; 
        
        file_buf = malloc(sizeof(char) * (buf_sz + 1));
        if (!file_buf)
            return NULL;
        
        // Go back to the start of the file.
        if (fseek(fp, 0L, SEEK_SET) != 0) {
            free(file_buf);
            file_buf = NULL;
        // Read the entire file into memory.
        } else {
            size_t total_bytes_read = fread(file_buf, sizeof(char), buf_sz, fp);
            if (ferror(fp) != 0 ) {
                free(file_buf);
                file_buf = NULL;
            } else {
                file_buf[total_bytes_read] = '\0';
                *buf_len = total_bytes_read;
            }
        }
    }
    fclose(fp);
    return file_buf;
}

// wrapper in case length is not needed (data is hex or base64 encoded).
char* get_string_from_file(const char* file_name) {
    int buf_len = 0;
    return get_buffer_from_file(file_name, &buf_len);
}

int main() {

    if (hamming_distance("this is a test", 14, "wokka wokka!!!", 14) != 37)
        return 0;

    char* file_buf = get_buffer_from_file("enc_file.txt");



    return 0;
}