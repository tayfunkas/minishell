NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBS = -lreadline
RM = rm -f

SRCS = $(addprefix source/, \
		$(addprefix parse/, init_token.c process_token.c process_operators.c add_token_to_list.c assign_token_type.c expand_token.c expand_utils.c) \
		$(addprefix handle/, handle.c pipes.c handle_cmd.c) \
		$(addprefix execute/external_cmd/, execute_ext_cmd.c check_ext_path.c find_cmd_path.c prepare_args.c fork.c fork_utils.c) \
		$(addprefix execute/internal_cmd/, init_int_cmd.c execute_int_cmd.c ft_cd.c ft_cd_helpers.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c set_env.c) \
		$(addprefix execute/redirection/, redir_with_cmd.c redir_without_cmd.c redir_heredoc.c redir_others.c ) \
		$(addprefix utils/, free.c check_sequence.c signal.c status.c check_syntax.c utils.c ) \
		$(addprefix libft/, ft_atoi.c ft_calloc.c ft_is.c ft_mem.c ft_split.c ft_str.c ft_strcmp.c ft_strcpy.c ft_strdup.c) \
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
