/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:16:06 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:16:09 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_cmd_path(char *paths, char *cmd)
{
	char	full_path[1024];

	ft_strcpy(full_path, paths);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

char	*get_path_env(char **our_env)
{
	int	i;

	i = 0;
	while (our_env[i] != NULL)
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
			return (our_env[i] + 5);
		i++;
	}
	perror("No PATH found.\n");
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;
	int		i;

	i = 0;
	path_env = get_path_env(our_env);
	if (!path_env)
		return (NULL);
	cmd_path = NULL;
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		cmd_path = build_cmd_path(paths[i], cmd);
		if (cmd_path)
			break ;
		i++;
	}
	free_split(paths);
	return (cmd_path);
}

