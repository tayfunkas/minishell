/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:37:55 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/24 17:57:12 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_internal_c(t_command *cmd, char ***env, t_exec_context *ctx)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd, cmd->argv[1], env, ctx));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd, env, ctx));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd, env));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*env));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd->argv));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd());
	else
	{
		write(2, "minishell: command not found 2\n", 31);
		return (127);
	}
}

static void	dup_and_close(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	execute_internal_commands(t_command *cmd, char ***env, t_exec_context *ctx)
{
	int	original_stdin;
	int	original_stdout;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
	{
		write(2, "minishell: command not found\n", 29);
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
	status = check_internal_c(cmd, env, ctx);
	dup_and_close(original_stdin, original_stdout);
	return (status);
}
