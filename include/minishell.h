/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:07:55 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/24 14:58:48 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include <sys/stat.h>
# include <sys/wait.h>

# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_signal
{
	int		sig_num;
}	t_signal;

extern t_signal	g_signal;

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
	int				quo_info;
	int				in_single_quotes;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**argv;
	int					argc;
	int					fd_in;
	int					fd_out;
	struct s_command	*next;
}	t_command;

typedef struct s_exec_context
{
	int		**pipe_fds;
	int		pipe_count;
	int		current_index;
	int		last_status;
	char	**our_env;
}	t_exec_context;

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
	int		token_idx;
}	t_expand;

typedef struct s_parser
{
	char	*start;
	char	*end;
	char	outer_quote;
	char	inner_quote;
	int		len;
}	t_parser;

//minishell.c
void		minishell(t_exec_context *ctx);
char		**copy_environment(char **envp);
void		free_environment(char **env);

//--------------------------------parse--------------------------------
//init_tokens.c
t_token		*tokenize_inputs(char *input, int max_args);
char		*skip_whitespace(char *start);

//process_tokens.c
t_token		*process_token(t_parser *parser, int *i, t_token *current);

//expand.c + expand_utils.c
void		expand_tokens(t_token *head, t_exec_context *ctx);
char		*expand_var(char *token, t_exec_context *ctx);
int			handle_quotes(char *token, t_expand *exp);
int			handle_env_var_exp(char *token, t_expand *exp, t_exec_context *ctx);

//add_token_to_list.c
t_token		*add_token_to_list(char *start, int len, char quote,
				t_token *current);

//parsing.c
void		assign_token_types(t_token *tokens);
void		type_tokens(t_token *token);

//-------------------------------handle-------------------------------
//handling.c
int			handle_tokens(t_token *tokens, t_exec_context *ctx);

//handle_pipe.c
int			count_pipes(t_token *tokens);
int			create_pipes(int **pipe_fds, int pipe_count);
void		free_pipe_fds(int **pipe_fds, int pipe_count);
void		close_pipes(int **pipe_fds, int pipe_count);

//handle_command.c
int			handle_command(t_token *current, t_token *cmd_end,
				t_exec_context *ctx, pid_t *pids);

//--------------------------execute command--------------------------
//execute_command.c
int			execute_command(t_token *start, t_token *end, t_exec_context *ctx);

//execute_ext_or_int.c
int			execute_ext_or_int(t_token *start, t_token *end, t_command *cmd,
				t_exec_context *ctx);

//check_ext_or_int.c
int			check_cmd_path(char *cmd_path, t_token *start, t_exec_context *ctx);
int			check_command_in_paths(char *cmd, char **paths);
int			is_external_command(char *cmd, char **our_env);
int			is_internal_command(char *cmd);

//-----------------------execute internal command-----------------------
//init_internal_cmd.c
t_command	*init_internal_command(t_token *current);

//execute_internal_cmd.c
int			execute_internal_commands(t_command *cmd, char ***env,
				t_exec_context *ctx);

//ft_cd.c, ft_echo.c, ft_env.c, ft_exit.c, ft_export.c, ft_pwd.c, ft_unset.c
int			ft_cd(char *path, char ***env, t_exec_context *ctx);
int			ft_echo(char **args);
int			ft_env(char **env);
int			ft_exit(char **args);
int			ft_export(t_command *cmd, char ***env, t_exec_context *ctx);
int			set_env(char ***env, const char *name, const char *value,
				t_exec_context *ctx);
int			ft_pwd(void);
int			ft_unset(t_command *cmd, char ***env);

//ft_cd_helpers.c
char		*get_env_value(char **env, const char *name);
void		home_directory(char **path, char *home);
void		handle_oldpwd(char **path, char ***env);
void		expand_tilde(char **path, char *home);
void		update_env(char ***env, char *current_dir, char *new_dir,
				t_exec_context *ctx);

//-----------------------execute external command-----------------------
//execute_external_commands.c
int			execute_external_commands(t_token *tokens, int token_count,
				t_command *cmd, t_exec_context *ctx);

//prepare_args.c
void		prep_args(t_token *tokens, int token_count, char **args,
				t_exec_context *ctx);
char		**allocate_args(int token_count);

//find_path.c
char		*find_cmd_path(char *cmd, char **our_env);
char		*get_path_env(char **our_env);

//fork_and_execute_external_command.c
int			fork_and_execute(t_command *cmd, char *cmd_path, char **args);

//-------------------------------redirection-------------------------------
//execute_redir.c + execute_redir_heredoc.c
void		setup_redir(t_token *start, t_token *end, int *fd_in, int *fd_out);
void		execute_redir_input(t_token *current, int *fd_in);
void		execute_redir_trunc(t_token *current, int *fd_out);
void		execute_redir_append(t_token *current, int *fd_out);
void		execute_redir_heredoc(t_token *current, int *fd_in);

//----------------------------------utils----------------------------------
//signal.c
void		setup_signal(void);
void		signal_handler(int sig);
void		check_signal_status(void);
void		handle_child_process_signals(void);

//status.c
void		update_last_status(t_exec_context *ctx, int status);
int			initialize_exit_status(t_exec_context *ctx);

//utils.c
void		free_tokens(t_token *tokens);
void		free_command(t_command *cmd);
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
char		*ft_strdup(char *s);
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_isdigit_str(char *c);
char		*ft_strstr(char *str, char *to_find);
int			ft_isdigit_str(char *c);
char		*ft_itoa(int num);
int			ft_isalnum(int c);
void		*ft_memcpy(void *dst, const void *src, size_t n);
char		*ft_strndup(const char *s1, size_t n);

#endif
