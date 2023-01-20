NAME = libcryptopals_static.a

CFLAGS = -Wall -Werror -Wextra

CC = gcc

LIB_DIR = cryptopals

SRC_DIR = $(LIB_DIR)/src
INC_DIR = $(LIB_DIR)/inc/

SRC = $(SRC_DIR)/format_conversions.c \
	  $(SRC_DIR)/logic_gates.c \
	  $(SRC_DIR)/hamming_distance.c \
	  $(SRC_DIR)/repeating_key_xor.c \
	  $(SRC_DIR)/rw_ops.c

OBJ = $(SRC:.c=.o)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rcs $(NAME) $?
	ranlib $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all bonus

.PHONY: all bonus clean fclean re
