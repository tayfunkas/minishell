/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:50:18 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/30 14:39:32 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *token)
{
	char	*env_var;

	if (token[0] == '$')
	{
		env_var = getenv(token + 1);
		if (env_var)
			return (ft_strdup(env_var));
		else
			return (ft_strdup(""));
	}
	return (ft_strdup(token));
}

void	prepare_args(t_token *tokens, int token_count, char **args)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (i < token_count && current)
	{
		args[i] = expand_var(current->str);
		current = current->next;
		i++;
	}
	args[token_count] = NULL;
}

void	handle_fork(pid_t pid, char *cmd_path, char **args)
{
	int	status;

	if (pid == 0)
	{
		if (execve(cmd_path, args, NULL) == -1)
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

void	execute_external_command(t_token *tokens, int token_count)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;

	cmd_path = find_cmd_path(tokens->str);
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
	prepare_args(tokens, token_count, args);
	args[token_count] = NULL;
	pid = fork();
	handle_fork(pid, cmd_path, args);
	free_external_commands(cmd_path, args, token_count);
}
