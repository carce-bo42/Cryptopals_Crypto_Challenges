NAME = libcryptopals.so

CFLAGS = -Wall -Werror -Wextra -fPIC -g3

CC = gcc

LIB_DIR = cryptopals

SRC_DIR = $(LIB_DIR)/src
INC_DIR = $(LIB_DIR)/inc/
OSSL_INC_DIR = openssl-3.0.7/include/

OSSL_LINK_DIR = openssl-3.0.7/

SRC = $(SRC_DIR)/format_conversions.c \
	  $(SRC_DIR)/logic_gates.c \
	  $(SRC_DIR)/hamming_distance.c \
	  $(SRC_DIR)/repeating_key_xor.c \
	  $(SRC_DIR)/rw_ops.c \
	  $(SRC_DIR)/ossl_core.c \
	  $(SRC_DIR)/aes_ecb.c \
	  $(SRC_DIR)/aes_cbc.c \
	  $(SRC_DIR)/matrix_ops.c

OBJ = $(SRC:.c=.o)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@  -I $(INC_DIR) -I $(OSSL_INC_DIR)

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -shared -o $(NAME) $? -L$(OSSL_LINK_DIR) -lcrypto

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all bonus

.PHONY: all bonus clean fclean re
