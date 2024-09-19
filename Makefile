NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = 

RM = rm -f

SRCS = parsing.c libft.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
