/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:11:05 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 13:11:40 by tkasapog         ###   ########.fr       */
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
