/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:07:55 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 13:47:03 by tkasapog         ###   ########.fr       */
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
	char			quote;
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

typedef struct s_exec_context
{
    int     **pipe_fds;
    int     pipe_count;
    int     current_index;
    char    **our_env;
}   t_exec_context;

//minishell.c
char		**copy_environment(char **envp);
void		free_environment(char **env);

//parsing.c
//t_token		*tokenize_input(char *input, int max_args);
t_token		*add_token_to_list(char *start, char *end, int in_quo, char quote, t_token *c);
t_token		*process_token(char **start, int *i, t_token *current);
char		*handle_quoted_string(char *start, char quote);
char		*skip_whitespace(char *start);
void		assign_token_types(t_token *tokens);
void		type_tokens(t_token *token);

//token_creation.c
t_token		*create_new_tok(char *start, int len, int in_quo, t_token *current);
char		*get_token_end(char *start, int *in_quo, char *quote);
char		*handle_regular_token(char *start);
char		*move_to_next_token(char *end, int in_quo, char quote);
t_token		*tokenize_input(char *input, int max_args);

//handling.c
void		handle_tokens(t_token *tokens, char **our_env);
int			count_tokens_until(t_token *start, t_token *end);
int			is_external_command(char *cmd, char **our_env);
int			is_internal_command(char *cmd);

//execute_command.c
void		execute_command(t_token *start, t_token *end, char **our_env);
int			count_tokens_until(t_token *start, t_token *end);

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
void		check_internal_c(t_command *cmd, char ***env);

//ft_cd_helpers.c
void		home_directory(char **path, char *home);
void		handle_oldpwd(char **path, char ***env);
void		expand_tilde(char **path, char *home);
void		update_env(char ***env, char *current_dir, char *new_dir);
int			validate_env_and_getcwd(char ***env, char *dir);

//execute_external_commands.c
void		execute_external_c(t_token *tokens, int token_count, t_command *cmd);
char		*expand_var(char *token, char **our_env, char quote);
void		prep_args(t_token *tokens, int token_count, char **args, char **our_env);
void		free_external_c(char *cmd_path, char **args, int token_count);

//external_command_helpers.c
char		**allocate_args(int token_count);
void		duplicate_fds(int fd_in, int fd_out);
void		restore_fds(int parent_in, int parent_out);
//void		handle_child_process(char *cmd_path, char **args);
void		fork_and_execute(t_command *cmd, char *cmd_path, char **args);

//path.c
char		*find_cmd_path(char *cmd, char **our_env);
char		*get_path_env(char **our_env);

//handle_pipe.c
void		handle_pipe(t_token *tokens, t_token *next_cmd, char **envp);
int			count_pipes(t_token *tokens);

//handle_token_pipes.c
void		free_pipe_fds(int **pipe_fds, int pipe_count);
int			create_pipes(int **pipe_fds, int pipe_count);
void		close_pipes(int **pipe_fds, int pipe_count);
void		setup_child_pipes(t_exec_context *ctx);
t_token 	*find_cmd_end(t_token *current);

//redicrectios.c
void		setup_redir(t_token *start, t_token *end, int *fd_in, int *fd_out);
void		execute_redir_heredoc(t_token *current, int *fd_in);

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
int			ft_isdigit_str(char *c);
char		*ft_strstr(char *str, char *to_find);
int			ft_isdigit_str(char *c);

#endif
