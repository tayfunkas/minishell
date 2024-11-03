NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBS = -lreadline
RM = rm -f

SRCS = $(addprefix source/, \
		$(addprefix parse/, add_token_to_list.c assign_token_type.c expand_token.c expand_utils.c parse.c) \
		$(addprefix handle/, handle.c pipes.c handle_cmd.c handle_redir.c) \
		$(addprefix execute/external_cmd/, execute_ext_cmd.c check_ext_path.c prepare_args.c fork.c fork_utils.c) \
		$(addprefix execute/internal_cmd/, init_int_cmd.c execute_int_cmd.c ft_cd.c ft_cd_helpers.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c set_env.c) \
		$(addprefix execute/redirection/, execute_redir.c) \
		$(addprefix utils/, free.c libft.c signal.c status.c syntax.c utils.c ) \
		minishell.c)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
		@echo "$(NAME) is successfully compiled!"

%.o: %.c
		$(CC) -g $(CFLAGS) $(INC) -c $< -o $@

clean:
		$(RM) $(OBJS)
		@echo "$(NAME) object files are cleaned!"

fclean:	clean
		$(RM) $(NAME)
		@echo "$(NAME) is cleaned!"

re: fclean all

.PHONY: all clean fclean re
