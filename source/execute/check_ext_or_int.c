/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ext_or_int.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:57:31 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/21 17:43:59 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd_path(char *cmd_path, t_command *cmd, t_token *start)
{
	struct stat	st;

	cmd_path = find_cmd_path(start->str, cmd->env);
	if (cmd_path == NULL && stat(start->str, &st) == -1)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, start->str, ft_strlen(start->str));
		write(2, "\n", 1);
		return (127);
	}
	if (cmd_path == NULL)
		cmd_path = ft_strdup(start->str);
	if (access(cmd_path, X_OK) == -1)
	{
		write(2, "minishell: permission denied: ", 30);
		write(2, start->str, ft_strlen(start->str));
		write(2, "\n", 1);
		free(cmd_path);
		return (126);
	}
	return (0);
}

int	check_command_in_paths(char *cmd, char **paths)
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
