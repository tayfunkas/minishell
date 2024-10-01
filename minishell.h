/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:07:55 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/30 14:40:24 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

//parsing.c
t_token		*tokenize_input(char *input, int max_args);
void		assign_token_types(t_token *tokens);
void		type_tokens(t_token *token);

//handling.c
void		handle_tokens(t_token *tokens, char **our_env);

//initiate_internal_commands.c
t_command	*init_internal_command(t_token *current, char **envp);

//execute_internal_commands.c
void		execute_internal_commands(t_command *cmd, char ***env);
void		ft_cd(char *path);
void		ft_pwd(void);
void		ft_echo(char **args);
void		ft_export(t_command *cmd, char ***env);
void		set_env(char ***env, const char *name, const char *value);
void		ft_unset(t_command *cmd, char ***env);
void		ft_env(char **env);
void		ft_exit(char **args);

//minishell.c
char	**copy_environment(char **envp);
void	free_environment(char **env);

//path.c
char		*find_cmd_path(char *cmd);

//execute_external_commands.c
void		execute_external_command(t_token *tokens, int token_count);

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
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
