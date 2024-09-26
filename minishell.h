#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>

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
t_token	*tokenize_input(char *input, int max_args);
void	assign_token_types(t_token *tokens);
void	type_tokens(t_token *token);

//handling.c
void	handle_tokens(t_token *tokens);
//int		is_external_command(char *cmd);
//int		is_internal_command(char *cmd);

//execution.c
void	execute_external_command(t_token *tokens, int token_count);

//utils.c
void	free_tokens(t_token *tokens);
void	free_split(char **paths);
int		count_tokens(t_token *tokens);

//libft.c
int		ft_isspace(int c);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
char	*ft_strcat(char *dest, char *src);
char	*ft_strcpy(char *s1, char *s2);
char	*ft_strdup(const char *s);

#endif
