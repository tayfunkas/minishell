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

void	ft_export(t_command *cmd, char *key_value)
{
	char	**key_value_pair;
	char	*key;
	char	*value;

	(void)cmd;
	if (!key_value)
	{
		write(2, "export: invalid argument\n", 25);
		return ;
	}
	key_value_pair = ft_split(key_value, '=');
	if (!key_value_pair || !key_value_pair[0] || !key_value_pair[1])
	{
		write(2, "export: invalid argument\n", 25);
		free(key_value_pair);
		return ;
	}
	key = key_value_pair[0];
	value = key_value_pair[1];
	if (setenv(key, value, 1) != 0)
		perror("export");
	free(key);
	free(value);
	free(key_value_pair);
}

void	ft_unset(t_command *cmd, char *key)
{
	(void)cmd;
	if (unsetenv(key) != 0)
		perror("unset");
}

void	ft_env(t_command *cmd)
{
	char	**envp;
	int		i;

	envp = cmd->env;
	i = 0;
	if (envp == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return ;
	}
	while (cmd->env[i] != NULL)
	{
		printf("%s\n", cmd->env[i]);
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

void	execute_internal_commands(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
	{
		write(2, "minishell: command not found\n", 29);
		return ;
	}
	//printf("Internal command: %s\n", cmd->argv[0]);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv[1]);
	//else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
	//	ft_pwd();
	//else if (ft_strcmp(cmd->argv[0], "echo") == 0)
	//	ft_echo(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ft_export(cmd, cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(cmd, cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(cmd->argv);
	else
		write(2, "minishell: command not found\n", 29);
}
