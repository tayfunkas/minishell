/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:50:18 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/26 13:47:16 by kyukang          ###   ########.fr       */
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
	if (!path_env) //if there is no path, return NULL
		return (NULL);
	cmd_path = NULL; //initiate cmd_path
	paths = ft_split(path_env, ':'); //split PATH with delimeter ':'
	i = 0;
	while (paths[i])
	{
		ft_strcpy(full_path, paths[i]); //Copy current path to 'full_path'
		ft_strcat(full_path, "/"); //Add "/" at the end of 'full_path'
		ft_strcat(full_path, cmd); //Add command at the end fo 'full_path'
		if (access(full_path, X_OK) == 0) //Check if there is a command in the path
		{
			cmd_path = ft_strdup(full_path); //Copy the full path
			break ; //If there is a command, we don't need to go through remaining paths. so terminate the loop.
		}
		i++;
	}
	free_split(paths);
	return (cmd_path);
}

/*
I changed 'cmd_path = find_cmd_path(tokens[0].str)' to 'cmd_path = find_cmd_path(tokens->str)'.
This make program go through not only the first token but all tokens as linked list and find the right command in the path.
*/

void	execute_external_command(t_token *tokens, int token_count)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	*args[token_count + 1];
	char	*envp[] = {NULL};
	int		i;
	t_token	*current;

	cmd_path = find_cmd_path(tokens->str);
	if (cmd_path == NULL)
		return ;
	i = 0;
	current = tokens;
	while (i < token_count && current)
	{
		args[i] = current->str;
		current = current->next;
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
