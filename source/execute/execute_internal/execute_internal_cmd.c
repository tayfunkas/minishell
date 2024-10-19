/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:37:55 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/19 19:40:04 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_internal_c(t_command *cmd, char ***env)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd->argv[1], env));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd, env));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd, env));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*env));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv));
	/*else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd->argv));*/
	else
	{
		write(2, "minishell: command not found 2\n", 31);
		return (127);
	}
}

int	execute_internal_commands(t_command *cmd, char ***env)
{
	int	original_stdin;
	int	original_stdout;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
	{
		write(2, "minishell: command not found 3\n", 31);
		return (127);
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
	status = check_internal_c(cmd, env);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (status);
}
