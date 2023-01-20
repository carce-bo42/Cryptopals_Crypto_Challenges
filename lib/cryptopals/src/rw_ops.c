#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this one returns a null terminated string.
char* get_buffer_from_file(const char* file_name, int *buf_len,
                           int newlines)
{
    char *file_buf = NULL;
    FILE *fp = fopen(file_name, "r");

    if (fp == NULL)
        return NULL;
    
    // Go to the end of the file.
    if (fseek(fp, 0L, SEEK_END) == 0) {

        // Get the size of the file.
        long buf_sz = ftell(fp);
        if (buf_sz == -1)
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
            // get whole file
            if (newlines) {
                size_t bytes_read = fread(file_buf, sizeof(char), buf_sz, fp);
                if (ferror(fp) != 0 ) {
                    free(file_buf);
                    file_buf = NULL;
                } else {
                    file_buf[bytes_read] = '\0';
                    *buf_len = bytes_read;
                }
            // get whole file without newlines
            } else {
                ssize_t read_bytes = 0;
                int offset = 0;
                char* ptr = NULL;
                size_t len = 0;
                while ((read_bytes = getline(&ptr, &len, fp)) != -1) {
                    if (ptr[read_bytes - 1] == '\n')
                        read_bytes -= 1;
                    memcpy(file_buf + offset, ptr, read_bytes);
                    offset += (int)read_bytes;
                }
                file_buf[offset] = '\0';
                *buf_len = offset - 1;
            }
        }
    }
    fclose(fp);
    return file_buf;
}

// wrapper in case length is not needed (data is hex or base64 encoded).
char* get_string_from_file(const char* file_name, int newlines) {
    int buf_len = 0;
    return get_buffer_from_file(file_name, &buf_len, newlines);
}