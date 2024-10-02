/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:39:22 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/02 14:18:41 by kyukang          ###   ########.fr       */
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

char	*find_cmd_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;
	int		i;

	path_env = getenv("PATH");
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
