/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:59:43 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/16 16:31:52 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	ft_cd(char *path, char ***env)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*home;
	char	*oldpwd;

	if (env == NULL || *env == NULL) 
	{
		printf("Error: env is NULL\n");
		return ;
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	home = get_env_value(*env, "HOME");
	if (path == NULL || path[0] == '\0' || ft_strcmp(path, "~") == 0)
	{
		path = home;
		if (path == NULL)
		{
			printf("Error: HOME not set\n");
			return ;
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		oldpwd = get_env_value(*env, "OLDPWD");
		if (oldpwd == NULL)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return ;
		}
		path = oldpwd;
		printf("%s\n", path);
	}
	else if (path[0] == '~' && path[1] == '/')
	{
		if (home == NULL)
		{
			printf("Error: HOME not set\n");
			return ;
		}
		ft_strcpy(new_dir, home);
		ft_strcat(new_dir, path + 1);
		path = new_dir;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	if (getcwd(new_dir, sizeof(new_dir)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	set_env(env, "OLDPWD", current_dir);
	set_env(env, "PWD", new_dir);
}*/

void	home_directory(char **path, char *home)
{
	if (*path == NULL || (*path)[0] == '\0' || ft_strcmp(*path, "~") == 0)
	{
		*path = home;
		if (*path == NULL)
			printf("Error: HOME not set \n");
	}
}

void	handle_oldpwd(char **path, char ***env)
{
	char	*oldpwd;

	oldpwd = get_env_value(*env, "OLDPWD");
	if (oldpwd == NULL)
		write(2, "cd: OLDPWD not set\n", 19);
	else
	{
		*path = oldpwd;
		printf("%s\n", *path);
	}
}

void	expand_tilde(char **path, char *home)
{
	char	new_dir[1024];

	if (home == NULL)
	{
		printf("Error: HOME not set\n");
		return ;
	}
	ft_strcpy(new_dir, home);
	strcat(new_dir, (*path) + 1);
	*path = new_dir;
}

void	update_env(char ***env, char *current_dir, char *new_dir)
{
	set_env(env, "OLDPWD", current_dir);
	set_env(env, "PWD", new_dir);
}

int	validate_env_and_getcwd(char ***env, char *dir)
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
