/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_command_exec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:37:55 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 10:38:21 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_key(char ***env, const char *key)
{
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
			while ((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			break ;
		}
		i++;
	}
}

void	ft_unset(t_command *cmd, char ***env)
{
	char	*key;

	key = cmd->argv[1];
	if (!key)
	{
		write(2, "unset: not enough arguments\n", 28);
		return ;
	}
	remove_env_key(env, key);
}

void	ft_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1] == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (!ft_isdigit_str(args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	if (args[2] != NULL)
	{
		write(1, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	status = ft_atoi(args[1]);
	printf("exit\n");
	exit(status);
}

void	execute_internal_commands(t_command *cmd, char ***env)
{
	int	original_stdin; 
	int	original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) 
	{
		write(2, "minishell: command not found\n", 29);
		return ;
	}
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	check_internal_c(cmd, env);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	check_internal_c(t_command *cmd, char ***env)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv[1], env);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
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
