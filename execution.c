/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:50:18 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/25 19:03:37 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//execute_external_command(), execute_internal_command()

static char	*find_cmd_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	char	*cmd_path;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	cmd_path = NULL;
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			cmd_path = ft_strdup(full_path);
			break ;
		}
		i++;
	}
	free_split(paths);
	return (cmd_path);
}

void	execute_external_command(t_token *tokens, int token_count)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	*args[token_count + 1];
	char	*envp[] = {NULL};
	int		i;

	cmd_path = find_cmd_path(tokens[0].str);
	if (cmd_path == NULL)
		return ;
	i = 0;
	while (i < token_count)
	{
		args[i] = tokens[i].str;
		i++;
	}
	args[token_count] = NULL;
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, envp) == -1)
			perror("Error from execve function\n");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("Error from fork\n");
	else
		waitpid(pid, &status, 0);
	free(cmd_path);
}