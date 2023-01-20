#ifndef CRYPTOPALS_RW_OPS_H
# define CRYPTOPALS_RW_OPS_H
# pragma once

char* get_buffer_from_file(const char* file_name, int *buf_len, int newlines);
char* get_string_from_file(const char* file_name, int newlines);

#endif 