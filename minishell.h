#ifndef MINISHELL
#define MINISHELL

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
	EMPTY,
	TRUNC,
	APPEND,
	INPUT,
	PIPE,
	END,
	CMD,
	ARG
} t_token_type;

typedef struct s_token
{
	char	*str;
	int	type;
	int	flag;
	struct s_token *prev;
	struct s_token *next;
} t_token;

typedef struct s_command
{
	char	**argv;
	int	argc;
	int	fd_in;
	int	fd_out;
	struct s_command *next;
} t_command;

//parsing.c
void	tokenize_input(char *input, char **args, int max_args, t_token *tokens);
void	execute_command(char **args);
void	type_arg(t_token *token);
//libft.c
char	*ft_strncpy(char *dest, char *src, unsigned int n);
char	*ft_strchr(const char *s, int c);
int	ft_isspace(int c);
int	ft_strcmp(char *s1, char *s2);

#endif
