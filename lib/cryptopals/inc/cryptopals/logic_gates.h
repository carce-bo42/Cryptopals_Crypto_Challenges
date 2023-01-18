#ifndef CRYPTOPALS_LOGIC_GATES_H
# define CRYPTOPALS_LOGIC_GATES_H
# pragma once

int compute_xor(const char* lhs, int lhs_size,
                const char* rhs, int rhs_size,
                char* result, int* result_len);

#endif 