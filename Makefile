NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline
INC =

RM = rm -f

SRCS = minishell.c parsing.c handling.c execution.c utils.c libft.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
