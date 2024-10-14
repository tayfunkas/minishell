NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBS = -lreadline
RM = rm -f

SRCS = $(addprefix parsing/, parsing.c handling.c) \
	   $(addprefix command/, external_commands.c init_internal_commands.c internal_commands.c path.c) \
	   $(addprefix pipes/, handle_pipe.c) \
	   $(addprefix utils/, libft.c utils.c) \
	   $(addprefix redirections/, redirections.c) \
	   minishell.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:
	make clean 

	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
