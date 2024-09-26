/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:58 by tkasapog          #+#    #+#             */
/*   Updated: 2024/09/26 14:59:14 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *path)
{
	if (path == NULL)
		write(2, "cd: missing argument\n", 21);
	else if (chdir(path) != 0)
		perror("cd");
}

void	ft_pwd()
{
	char	cwd[1024];

	if(getcwd(cwd, sizeof(cwd)) != NULL)
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

void	ft_export(char *key_value)
{
	char	**key_value_pair;
	char	*key;
	char	*value;	

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

void	ft_unset(char *key)
{
	if (unsetenv(key) != 0)
		perror("unset");
}

void	ft_env(char **envp)
{
	int	i;
	
	i = 0;
	
	if (envp == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return ;
	}
	while(envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

void	ft_exit(char **args)
{
	int	status;
	
	status = 0;
	if(args[1] != NULL)
		status = ft_atoi(args[1]);
	exit(status);
}

void	execute_internal_commands(t_command *cmd, char **envp)
{
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) 
	{
        	write(2, "minishell: command not found\n", 29);
        	return;
    	}
    	printf("Internal command: %s\n", cmd->argv[0]);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ft_echo(cmd->argv);
	else if	(ft_strcmp(cmd->argv[0], "export") == 0)
		ft_export(cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ft_env(envp);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(cmd->argv);
	else
		write(2, "minishell: command not found\n", 29);
}
