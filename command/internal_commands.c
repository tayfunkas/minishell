/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:58 by tkasapog          #+#    #+#             */
/*   Updated: 2024/09/30 14:41:44 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *path)
{
	char	*home;
	char	current_dir[1024];
	char	new_dir[1024];
	char	*oldpwd;

	if (path == NULL)
		printf("ft_cd received path: (null)\n");
	else
		printf("ft_cd received path: %s\n", path);
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	printf("Current directory before change: %s\n", current_dir);
	home = getenv("HOME");
	if (path == NULL || path[0] == '\0')
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			write(2, "cd: HOME not set\n", 17);
			return ;
		}
	}
	if (path == NULL || path[0] == '\0' || ft_strcmp(path, "~") == 0)
		path = home;
	else if (path[0] == '-')
	{
		oldpwd = getenv("OLDPWD");
		path = oldpwd;
		printf("%s\n", path);
	}
	else if (path[0] == '~')
	{
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
	printf("New directory after change: %s\n", new_dir);
	setenv("OLDPWD", current_dir, 1);
	setenv("PWD", new_dir, 1);
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	ft_echo(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (args[1] != NULL && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
		{
			printf(" ");
		}
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_export(t_command *cmd, char ***env)
{
	char	*key_value;
	char	*equals_sign;
	char	*key;
	char	*value; 
	
	key_value = cmd->argv[1];
	equals_sign = ft_strchr(key_value, '=');
	
	if (cmd->argc < 2)
	{
		write(2, "export: not enough arguments\n", 29);
		return ;
	}
	if (equals_sign)
	{
		*equals_sign = '\0';
		key = key_value;
		value = equals_sign + 1;
		set_env(env, key, value);
		*equals_sign = '=';
	}
	else
		set_env(env, key_value, "");
}

void	set_env(char ***env, const char *name, const char *value)
{
	int	i;
	int	len;
	char	*new_var;
	
	i = 0;
	len = ft_strlen(name);
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], name, len) == 0 && (*env)[i][len] == '=')
		{
			free(*env[i]);
			new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
			if (!new_var)
			{
				perror("malloc");
				return ; 
			}
			ft_strcpy(new_var, name);
			ft_strcat(new_var, "=");
			ft_strcat(new_var, value);
			(*env)[i] = new_var;
			return ;
		}
		i++;
	}
	*env = realloc(*env, sizeof(char*) * (i + 2));
	if (!*env)
	{
		perror("malloc");
		return ;
	}
	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!new_var)
			{
				perror("malloc");
				return ; 
			}
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	(*env)[i] = new_var;
	(*env)[i + 1] = NULL;
}
	
void	ft_unset(t_command *cmd, char ***env)
{
	char	*key;
	
	key = cmd->argv[1];
	if (!key)
	{
		write(2, "unset: not enough arguments\n", 28);
		return;
	}
	
	int	i;
	int	j;
	int	len;
	
	i = 0;
	len = ft_strlen(key);
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], key, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			j = i;
			while((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	ft_env(char **env)
{
	int	i;
	
	i = 0;
	if (env == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return ;
	}
	while(env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	ft_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1] != NULL)
		status = ft_atoi(args[1]);
	exit(status);
}

void	execute_internal_commands(t_command *cmd, char ***env)
{
	int	i;
	
	i = 0;	
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) 
	{
        	write(2, "minishell: command not found\n", 29);
        	return;
    	}
    	//printf("Internal command: %s\n", cmd->argv[0]);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv[1]);
	/*else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ft_echo(cmd->argv);*/
	else if	(ft_strcmp(cmd->argv[0], "export") == 0)
		ft_export(cmd, env);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(cmd, env);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ft_env(*env);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(cmd->argv);
	else
		write(2, "minishell: command not found\n", 29);
}

