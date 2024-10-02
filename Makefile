NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline
INC =

RM = rm -f

SRCS = minishell.c \
	${addprefix $(parsing/), parsing.c handling.c} \
	${addprefix $(command/), execution.c external_command.c init_internal_commands.c internal_commands.c path.c} \
	${addprefix $(utils/), libft.c utils.c}

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
