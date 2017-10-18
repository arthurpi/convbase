CC = gcc
RM = rm -rf

NAME = convbase

SRC = conv.c
OBJ = $(SRC:%.c=%.o)

CFLAGS = -Wall -Wextra -O2 -s

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean fclean re
