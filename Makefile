NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBS = -lreadline
RM = rm -f

SRCS = $(addprefix source/, \
		$(addprefix parse/, init_tokens.c process_tokens.c assign_token_type.c add_token_to_list.c expand.c expand_utils.c) \
		$(addprefix handle/, handle.c handle_pipe.c handle_command.c) \
		$(addprefix execute/, execute_command.c execute_ext_or_int.c check_ext_or_int.c \
		$(addprefix execute_internal/, init_internal_cmd.c execute_internal_cmd.c ft_cd.c ft_cd_helpers.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c set_env.c) \
		$(addprefix execute_external/, execute_external_cmd.c find_path.c fork_and_execute_extcmd.c prepare_args.c) \
		$(addprefix execute_redirection/, execute_redir.c execute_redir_heredoc.c)) \
		$(addprefix utils/, libft.c utils.c signal.c status.c syntax.c) \
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
