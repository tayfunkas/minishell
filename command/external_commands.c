/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:50:18 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/04 13:43:05 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *token, char **our_env)
{
	char	*env_var;
	int		i;

	i = 0;
	if (token[0] == '$')
	{
		env_var = token + 1;
		while (our_env[i])
		{
			if (ft_strncmp(our_env[i], env_var, ft_strlen(env_var)) == 0 && our_env[i][ft_strlen(env_var)] == '=')
				return ft_strdup(our_env[i] + ft_strlen(env_var) + 1);
			i++;
		}
		return (ft_strdup(""));
	}
	return (ft_strdup(token));
}

void	prepare_args(t_token *tokens, int token_count, char **args, char **our_env)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (i < token_count && current)
	{
		args[i] = expand_var(current->str, our_env);
		current = current->next;
		i++;
	}
	args[token_count] = NULL;
}

void	handle_fork(pid_t pid, char *cmd_path, char **args, char **our_env)
{
	int	status;

	if (pid == 0)
	{
		if (execve(cmd_path, args, our_env) == -1)
			perror("Error from execve function\n");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("Error from fork\n");
	else
		waitpid(pid, &status, 0);
}

void	free_external_commands(char *cmd_path, char **args, int token_count)
{
	int	i;

	free(cmd_path);
	i = 0;
	while (i < token_count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	execute_external_command(t_token *tokens, int token_count, char **our_env)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;

	cmd_path = find_cmd_path(tokens->str, our_env);
	if (cmd_path == NULL)
	{
		perror("Command not found\n");
		return ;
	}
	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (args == NULL)
	{
		perror("malloc failed\n");
		free(cmd_path);
		return ;
	}
	prepare_args(tokens, token_count, args, our_env);
	args[token_count] = NULL;
	pid = fork();
	handle_fork(pid, cmd_path, args, our_env);
	free_external_commands(cmd_path, args, token_count);
}
