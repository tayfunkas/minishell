/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/16 14:57:11 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int	is_external_command(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = NULL;
	i = 0;
	if (ft_strcmp(cmd, "env") == 0)
		return (0);
	while (our_env[i])
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (1);
		}
		i++;
	}
	free_split(paths);
	return (0);
}*/

static char	*get_path_env(char **our_env)
{
	int		i;
	char	*path_env;

	i = 0;
	path_env = NULL;
	while (our_env[i])
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break ;
		}
		i++;
	}
	return (path_env);
}

static int	check_command_in_paths(char *cmd, char **paths)
{
	int		i;
	char	full_path[1024];

	i = 0;
	while (paths[i] != NULL)
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_external_command(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	int		result;

	if (ft_strcmp(cmd, "env") == 0)
		return (0);
	path_env = get_path_env(our_env);
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (0);
	result = check_command_in_paths(cmd, paths);
	free_split(paths);
	return (result);
}

int	is_internal_command(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	handle_tokens(t_token *tokens, char **our_env)
{
	int		pipe_count;
	int		**pipe_fds;
	pid_t	*pids;
	t_token	*current;
	int		i;

	pipe_count = count_pipes(tokens);
	pipe_fds = malloc(sizeof(int *) * pipe_count);
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	current = tokens;
	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			//free allocated memory
			return ;
		}
		i++;
	}
	i = 0;
	while (i <= pipe_count)
	{
		t_token *cmd_end = current;
		while (cmd_end && cmd_end->type != PIPE)
			cmd_end = cmd_end->next;
		if (is_internal_command(current->str) && pipe_count == 0)
			execute_command(current, cmd_end, our_env);
		else
		{
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("fork");
				//free allocated memory
				return ;
			}
			else if (pids[i] == 0)
			{
				if (i > 0)
					dup2(pipe_fds[i-1][0], STDIN_FILENO);
				if (i < pipe_count)
					dup2(pipe_fds[i][1], STDOUT_FILENO);

				int j = 0;
				while (j < pipe_count)
				{
					close(pipe_fds[j][0]);
					close(pipe_fds[j][1]);
					j++;
				}
				execute_command(current, cmd_end, our_env);
				exit(EXIT_SUCCESS);
			}
		}
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		i++;
	}
	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pids);
}


