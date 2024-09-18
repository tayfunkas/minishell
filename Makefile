NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = 

RM = rm -f

SRCS:

OBJS:

all: $(NAME)

$(NAME):

%.o: %.c

clean:

fclean:

re: fclean all

.PHONY: all clean fclean re
