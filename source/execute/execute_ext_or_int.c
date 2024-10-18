/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_or_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:43:31 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:49:56 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	count_tokens_until(t_token *start, t_token *end)
{
	int		count;
	t_token	*current;

	count = 0;
	current = start;
	while (current != end && current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	execute_ext_or_int(t_token *start, t_token *end, t_command *cmd)
{
	int			arg_count;
	t_command	*internal_cmd;
	int			status;

	if (is_external_command(start->str, cmd->env))
	{
		arg_count = count_tokens_until(start, end);
		status = execute_external_commands(start, arg_count, cmd);
		return (status);
	}
	else if (is_internal_command(start->str))
	{
		internal_cmd = init_internal_command(start, cmd->env);
		if (!internal_cmd)
			return (1);
		internal_cmd->fd_in = cmd->fd_in;
		internal_cmd->fd_out = cmd ->fd_out;
		status = execute_internal_commands(internal_cmd, &cmd->env);
		free_command(internal_cmd);
		return (status);
	}
	write(2, "minishell: command not found\n", 29);
	return (127); 
}
