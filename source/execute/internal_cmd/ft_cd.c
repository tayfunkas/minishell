/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:05:51 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:25 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_env_and_getcwd(char ***env, char *dir)
{
	if (env == NULL || *env == NULL)
	{
		printf("Error: env is NULL\n");
		return (-1);
	}
	if (getcwd(dir, 1024) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	return (0);
}

static int	handle_path(char *path, char ***env, char *home)
{
	if (path && ft_strcmp(path, "-") == 0)
		handle_oldpwd(&path, env);
	else if (path && path[0] == '~' && path[1] == '/')
		expand_tilde(&path, home);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	ft_cd(t_cmd *cmd, char *path, char ***env, t_ctx *ctx)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*home;

	if (cmd->argc > 2)
	{
		write(2, "cd : too many arguments\n", 24);
		return (1);
	}
	if (validate_env_and_getcwd(env, current_dir) == -1)
		return (1);
	home = get_env_value(*env, "HOME");
	home_directory(&path, home);
	if (handle_path(path, env, home) == 1)
		return (1);
	if (getcwd(new_dir, sizeof(new_dir)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	update_env(env, current_dir, new_dir, ctx);
	return (0);
}
