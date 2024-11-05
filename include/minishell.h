/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:43:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 03:17:57 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <termio.h>
# include <unistd.h>

# include <sys/stat.h>
# include <sys/wait.h>

# include <readline/readline.h>
# include <readline/history.h>

extern int	g_signal;

typedef enum e_token_type
{
	FILENAME,
	EMPTY,
	TRUNC,
	APPEND,
	INPUT,
	HEREDOC,
	PIPE,
	END,
	CMD,
	ARG
}	t_tok_type;

typedef struct s_tok
{
	char			*str;
	int				type;
	int				flag;
	char			quote;
	int				in_single_quotes;
	struct s_tok	*prev;
	struct s_tok	*next;
}	t_tok;

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	int				fd_in;
	int				fd_out;
	char			*cmd_path;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_ctx
{
	int		**pipe_fds;
	int		pipe_count;
	int		current_index;
	int		last_status;
	int		syntax_error;
	char	**our_env;
}	t_ctx;

typedef struct s_expand
{
	char	*result;
	size_t	result_size;
	int		result_idx;
	char	*new_result;
	char	*env_var;
	int		env_index;
	char	*env_value;
	int		in_single_quote;
	int		in_double_quote;
	int		tok_idx;
}	t_expand;

typedef struct s_env
{
	char	*key;
	char	*value;
	char	*key_value;
}	t_env;

typedef struct s_parser
{
	char	*start;
	char	*end;
	char	outer;
	char	inner;
	int		len;
	char	unclosed;
}	t_parser;

typedef struct s_quote
{
	char	outer;
	char	inner;
	int		i;
	int		j;
}	t_quote;

typedef struct s_heredoc
{
	char	*delim;
	int		pipe_fd[2];
}	t_heredoc;

//..........................parse........................
//init_token.c
t_tok	*tokenize_inputs(char *input, int max_args);
//process_token.c
int		process_token(t_parser *pars, int *i, t_tok **current, t_tok **head);
//t_tok	*process_token(t_parser *pars, int *i, t_tok *current);
//process_operators.c
t_tok	*process_initial_operators(t_parser *pars, int *i, t_tok *current);
//assign_token_type.c
void	assign_token_types(t_tok *tokens);
//add_token_to_list.c
t_tok	*tok_to_list(char *start, int len, char quote, t_tok *current);
//expand_token.c
void	expand_tokens(t_tok *head, t_ctx *ctx);
char	*expand_var(char *token, t_ctx *ctx);
int		handle_quotes(char *token, t_expand *exp);
int		handle_env_var_exp(char *tok, t_expand *exp, t_ctx *ctx);

//..........................handle........................
//handle.c
int		handle_tokens(t_tok *tokens, t_ctx *ctx);
//handle_cmd.c
int		handle_cmd(t_tok *current, t_tok *cmd_end, t_ctx *ctx);
int		setup_cmd_fds(t_cmd *cmd, t_tok *start, t_tok *end, t_ctx *ctx);
//pipes.c
int		count_pipes(t_tok *tokens);
int		create_pipes(int **pipe_fds, int pipe_count);
void	close_pipes(int **pipe_fds, int pipe_count);
void	free_pipe_fds(int **pipe_fds, int pipe_count);
void	setup_child_pipes(t_ctx *ctx);

//..........................execute external command........................
//execute_ext_cmd.c
int		execute_external_cmd(t_ctx *ctx, t_tok *start, t_tok *end);
//check_ext_path.c
int		check_cmd_path(char **cmd_path, t_tok *start, t_ctx *ctx);
int		check_cmd_in_paths(char *cmd, char **paths);
//find_cmd_path.c
char	*find_cmd_path(char *cmd, char **our_env);
//prepare_args.c
char	**allocate_args(int token_count);
void	prep_args(t_tok *tokens, int token_count, char **args, t_ctx *ctx);
//fork.c + fork_utils.c
int		fork_and_execute(t_cmd *cmd, t_ctx *ctx, t_tok *start, t_tok *end);
void	restore_fds(int parent_in, int parent_out);
void	duplicate_fds(int fd_in, int fd_out);

//..........................execute internal command........................
//init_int_cmd.c
t_cmd	*init_internal_cmd(t_tok *current);
//execute_int_cmd.c
int		execute_internal_cmd(t_ctx *ctx, t_tok *start, t_tok *end);
//ft_cd.c + helpers.c
int		ft_cd(t_cmd *cmd, char *path, char ***env, t_ctx *ctx);
char	*get_env_value(char **env, const char *name);
void	expand_tilde(char **path, char *home);
void	handle_oldpwd(char **path, char ***env);
void	home_directory(char **path, char *home);
void	update_env(char ***env, char *current_dir, char *new_dir, t_ctx *ctx);
//ft_echo.c
int		ft_echo(char **args, t_tok *tokens);
//ft_env.c
int		ft_env(char **env, t_cmd *cmd);
//ft_exit.c
int		ft_exit(char **args, t_ctx *ctx, t_tok *tokens, t_cmd *cmdlist);
//ft_export.c
int		ft_export(t_cmd *cmd, char ***env, t_ctx *ctx);
//ft_pwd.c
int		ft_pwd(void);
//ft_unset.c
int		ft_unset(t_cmd *cmd, char ***env);
//set_env.c
int		set_env(char ***env, const char *name, const char *value, t_ctx *ctx);

//..........................execute redirections........................
//redir_with_cmd.c
int		setup_redir(t_tok *start, t_tok *end, t_ctx *ctx, t_cmd *cmd);
//redir_without_cmd.c
int		setup_redir_only(t_tok *start, t_tok *end, t_ctx *ctx);
//redir_heredoc.c
void	handle_heredoc_parent(int *pipe_fd, int *fd_in, pid_t pid, char *delim);
void	collect_heredoc_delimiters(t_tok *start, t_tok *end, char **delim);
int		line_or_delim(char *delim, char *line);
void	heredoc_pid_error(int *pipe_fd, char *delim);
//redir_others.c
int		execute_redir_input(t_tok *current, int *fd_in);
int		execute_redir_append(t_tok *current, int *fd_out);
int		execute_redir_trunc(t_tok *current, int *fd_out);

//.................................utils...............................
//check_sequence.c
int		check_invalid_sequences(const char *input);
//check_syntax.c
int		check_syntax(t_tok *tokens);
//free.c
void	free_environment(char **env);
void	free_context(t_ctx *ctx);
void	free_command_list(t_cmd *head);
void	free_command(t_cmd *cmd);
void	free_tokens(t_tok *tokens);
//signal.c
void	parent_sigint_handler(int sig);
void	child_sigint_handler(int sig);
void	child_sigquit_handler(int sig);
void	setup_signal(void);
//status.c
int		initialize_exit_status(t_ctx *ctx);
void	update_last_status(t_ctx *ctx, int status);

//utils.c
char	**copy_environment(char **envp);
void	write_error(const char *message_prefix, const char *command);

//.................................libft...............................
//ft_atoi.c
int		ft_atoi(const char *str);
char	*ft_itoa(int num);
//ft_calloc.c
void	*ft_calloc(size_t nmemb, size_t size);
//ft_is.c
int		ft_isspace(int c);
int		ft_isalnum(int c);
int		ft_isdigit_str(char *c);
//ft_mem.c
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
//ft_split.c
char	**ft_split(char const *s, char c);
void	free_split(char **paths);
//ft_str.c
char	*ft_strstr(char *str, char *to_find);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strcat(char *dest, const char *src);
//ft_strcmp.c
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
//ft_strcpy.c
char	*ft_strcpy(char *s1, const char *s2);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
//ft_strdup.c
char	*ft_strdup(char *s);
char	*ft_strndup(const char *s1, size_t n);

#endif
