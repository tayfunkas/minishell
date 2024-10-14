/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:07:55 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/14 21:19:03 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

typedef enum e_token_type
{
	EMPTY,
	TRUNC,
	APPEND,
	INPUT,
	HEREDOC,
	PIPE,
	END,
	CMD,
	ARG
}	t_token_type;

typedef struct s_token
{
	char			*str;
	int				type;
	int				flag;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**argv;
	int					argc;
	int					fd_in;
	int					fd_out;
	char				**env;
	struct s_command	*next;
}	t_command;

//minishell.c
char		**copy_environment(char **envp);
void		free_environment(char **env);

//parsing.c
//t_token		*tokenize_input(char *input, int max_args);
t_token		*add_token_to_list(char *start, char *end, int in_quo, t_token *c);
t_token	*process_token(char **start, int *i, t_token *current);
char *handle_quoted_string(char *start, char quote);
char *skip_whitespace(char *start);
void		assign_token_types(t_token *tokens);
void		type_tokens(t_token *token);
//token_creation.c
t_token *create_new_token(char *start, int len, int inside_quotes, t_token *current);
char *get_token_end(char *start, int *inside_quotes, char *quote);
char	*handle_regular_token(char *start);
char *move_to_next_token(char *end, int inside_quotes, char quote);
t_token *tokenize_input(char *input, int max_args);


//handling.c
void		handle_tokens(t_token *tokens, char **our_env);
int			count_tokens_until(t_token *start, t_token *end);
void		execute_command(t_token *start, t_token *end, char **our_env);

//initiate_internal_commands.c
t_command	*init_internal_command(t_token *current, char **envp);

//execute_internal_commands.c
void		execute_internal_commands(t_command *cmd, char ***env);
void		ft_cd(char *path, char ***env);
void		ft_pwd(void);
void		ft_echo(char **args);
void		ft_export(t_command *cmd, char ***env);
void		set_env(char ***env, const char *name, const char *value);
void		ft_unset(t_command *cmd, char ***env);
void		ft_env(char **env);
void		ft_exit(char **args);
char		*get_env_value(char **env, const char *name);

//execute_external_commands.c
//void		execute_external_command(t_token *tokens, int token_count, char **our_env);
void execute_external_command(t_token *tokens, int token_count, char **our_env, int fd_in, int fd_out);
char		*expand_var(char *token, char **our_env);
void		prepare_args(t_token *tokens, int token_count, char **args, char **our_env);
void		handle_fork(pid_t pid, char *cmd_path, char **args, char **our_env);
void		free_external_commands(char *cmd_path, char **args, int token_count);

//path.c
char		*find_cmd_path(char *cmd, char **our_env);

//handle_pipe.c
void		handle_pipe(t_token *tokens, t_token *next_cmd, char **envp);
int			count_pipes(t_token *tokens);
//redicrectios.c
void	setup_redirect(t_token *start, t_token *end, int *fd_in, int *fd_out);
//utils.c
void		free_tokens(t_token *tokens);
void		free_command(t_command *cmd);;
void		free_split(char **paths);
int			count_tokens(t_token *tokens);

//libft.c
int			ft_isspace(int c);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strncpy(char *dest, char *src, unsigned int n);
char		*ft_strcat(char *dest, const char *src);
char		*ft_strcpy(char *s1, const char *s2);
char		*ft_strdup(const char *s);
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isdigit_str(char *c);
char	*ft_strstr(char *str, char *to_find);
int	ft_isdigit_str(char *c);

#endif
