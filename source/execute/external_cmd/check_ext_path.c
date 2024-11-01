/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ext_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:27:54 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 14:44:53 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_cmd_in_paths(char *cmd, char **paths)
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

static int	if_path_is_null(char **cmd_path, t_token *start, struct stat st)
{
	if (*cmd_path == NULL)
	{
		if (start->str[0] == '/'
			|| (start->str[0] == '.' && start->str[1] == '/'))
		{
			if (stat(start->str, &st) == -1)
			{
				write_error("minishell: no such file or directory: ",
					start->str);
				return (127);
			}
			if (S_ISDIR(st.st_mode))
			{
				write_error("minishell: is a directory: ", start->str);
				return (126);
			}
			*cmd_path = ft_strdup(start->str);
		}
		else
		{
			write_error("minishell: command not found: ", start->str);
			return (127);
		}
	}
	return (0);
}

int	check_cmd_path(char **cmd_path, t_token *start, t_exec_context *ctx)
{
	struct stat	st;
	int			status;

	ft_memset(&st, 0, sizeof(struct stat));
	*cmd_path = find_cmd_path(start->str, ctx->our_env);
	status = if_path_is_null(cmd_path, start, st);
	if (status != 0)
		return (status);
	if (access(*cmd_path, X_OK) == -1)
	{
		if (S_ISDIR(st.st_mode))
		{
			write_error("minishell: is a directory: ", start->str);
			return (126);
		}
		write_error("minishell: permission denied: ", start->str);
		free(*cmd_path);
		return (126);
	}
	return (0);
}

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
