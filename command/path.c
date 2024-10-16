/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:39:22 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/16 14:50:44 by tkasapog         ###   ########.fr       */
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

char	*find_cmd_path(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;
	int		i;

	i = 0;
	path_env = NULL;
	while (our_env[i] != NULL)
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
	{
		perror("No PATH found.\n");
		return (NULL);
	}
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
