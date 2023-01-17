#ifndef CRYPTOPALS_TOOLS_H
# define CRYPTOPALS_TOOLS_H
# pragma once

int hex_to_bin(const char* hex, char* bin, int *bin_len);
char* bin_to_hex(const char* bin, int bin_len);

int get_base64_len(int bin_len);
char* hex_to_base64(const char* hex);

#endif 