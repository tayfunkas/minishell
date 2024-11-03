
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
}	t_token_type;

typedef struct s_token
{
	char			*str;
	int				type;
	int				flag;
	char			quote;
	int				quo_info;
	int				in_single_quotes;
	int				pipe_count;
	int				cmd_count;
	int				redir_count;
	int				cmd_flag;
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
	int		syntax_error;
	char	**our_env;
	//pid_t	*child_pid;
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
	char	outer_quote;
	char	inner_quote;
	int		len;
	char	unclosed_quote;
}	t_parser;

void		free_external_c(char *cmd_path, char **args, int token_count);
//--------------------------------parse--------------------------------
//init_tokens.c
t_token		*tokenize_inputs(char *input, int max_args);
t_token		*process_token(t_parser *parser, int *i, t_token *current);

//add_token_to_list.c
t_token		*add_token_to_list(char *start, int len, char quote, t_token *current);

//expand_token.c
void		expand_tokens(t_token *head, t_exec_context *ctx);
char		*expand_var(char *token, t_exec_context *ctx);
int			handle_env_var_exp(char *token, t_expand *exp, t_exec_context *ctx);
int			handle_quotes(char *token, t_expand *exp);

//assign_token_type.c
void		assign_token_types(t_token *tokens);

//--------------------------------handle--------------------------------
//handle.c
int			handle_tokens(t_token *tokens, t_exec_context *ctx);

//handle_cmd.c
int			handle_cmd(t_token *current, t_token *cmd_end, t_exec_context *ctx);

//handle_redir.c
int			handle_initial_redir(t_token **current, int *fd_in);

//pipe.c
int			count_pipes(t_token *tokens);
int			create_pipes(int **pipe_fds, int pipe_count);
void		close_pipes(int **pipe_fds, int pipe_count);
void		free_pipe_fds(int **pipe_fds, int pipe_count);
void		setup_child_pipes(t_exec_context *ctx);
void		setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx);

//-------------------------------execute_ext--------------------------------
//execute_ext
int			execute_external_cmd(t_exec_context *ctx, t_token *start, t_token *end);

//check_path
char		*get_path_env(char **our_env);
int			check_cmd_in_paths(char *cmd, char **paths);
int			check_cmd_path(char **cmd_path, t_token *start, t_exec_context *ctx);
char		*find_cmd_path(char *cmd, char **our_env);

//allocate & prepare args
void		prep_args(t_token *tokens, int token_count, char **args, t_exec_context *ctx);
char		**allocate_args(int token_count);

//fork + utils
int			fork_and_execute(t_command *cmd, char *cmd_path, char **args, t_exec_context *ctx, t_token *start, t_token *end);
void		duplicate_fds(int fd_in, int fd_out);
void		restore_fds(int parent_in, int parent_out);
void		check_fds(t_command *cmd);

//-------------------------------execute_int--------------------------------
t_command	*init_internal_cmd(t_token *current);

int			execute_internal_cmd(t_exec_context *ctx, t_token *start, t_token *end, t_token *tokens);

int			ft_cd(t_command *cmd, char *path, char ***env, t_exec_context *ctx);
//int			ft_echo(char **args);
int			ft_echo(char **args, t_token *tokens);
int			ft_env(char **env, t_command *cmd);
int			ft_exit(char **args, t_exec_context *ctx, t_token *tokens, t_command *cmdlist);
int			ft_export(t_command *cmd, char ***env, t_exec_context *ctx);
int			ft_pwd(void);
int			ft_unset(t_command *cmd, char ***env);
int			set_env(char ***env, const char *name, const char *value, t_exec_context *ctx);

//fd_cd helpers
char		*get_env_value(char **env, const char *name);
void		home_directory(char **path, char *home);
void		handle_oldpwd(char **path, char ***env);
void		expand_tilde(char **path, char *home);
void		update_env(char ***env, char *current_dir, char *new_dir, t_exec_context *ctx);

//--------------------------------execute_redir--------------------------------
void		setup_redir(t_token *start, t_token *end, int *fd_in, int *fd_out);
void		execute_redir_input(t_token *current, int *fd_in);
void		execute_redir_trunc(t_token *current, int *fd_out);
void		execute_redir_append(t_token *current, int *fd_out);
void		execute_redir_heredoc(t_token *current, int *fd_in);

//--------------------------------utils--------------------------------
//syntax.c
int			check_syntax(t_token *tokens, t_exec_context *ctx);

//status.c
int			initialize_exit_status(t_exec_context *ctx);
void		update_last_status(t_exec_context *ctx, int status);

//signal.c
void		parent_sigint_handler(int sig);
void		child_sigint_handler(int sig);
void		child_sigquit_handler(int sig);
void		setup_signal(void);

//utils.c
void		write_error(const char *message_prefix, const char *command);
char		**copy_environment(char **envp);

//free.c
void		free_environment(char **env);
void		free_context(t_exec_context *ctx);
void		free_expand(t_expand *exp);
void		free_command_list(t_command *head);
void		free_tokens(t_token *tokens);
void		free_split(char **paths);
void		free_command(t_command *cmd);

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
void		*ft_memset(void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);

#endif
