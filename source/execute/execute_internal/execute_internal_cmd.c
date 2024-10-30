/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:37:55 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/30 18:53:48 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_internal_c(t_master *master)
{
	if (ft_strcmp(master->cmd->argv[0], "cd") == 0)
		return (ft_cd(master->cmd, master->cmd->argv[1], &master->ctx->our_env, master->ctx));
	else if (ft_strcmp(master->cmd->argv[0], "export") == 0)
		return (ft_export(master->cmd, &master->ctx->our_env, master->ctx));
	else if (ft_strcmp(master->cmd->argv[0], "unset") == 0)
		return (ft_unset(master->cmd, &master->ctx->our_env));
	else if (ft_strcmp(master->cmd->argv[0], "env") == 0)
		return (ft_env(master->ctx->our_env, master->cmd));
	else if (ft_strcmp(master->cmd->argv[0], "exit") == 0)
		return (ft_exit(master->cmd->argv, master));
	else if (ft_strcmp(master->cmd->argv[0], "echo") == 0)
		return (ft_echo(master->cmd->argv));
	else if (ft_strcmp(master->cmd->argv[0], "pwd") == 0)
		return (ft_pwd());
	else
		return (127);
}

static void	dup_and_close(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	execute_internal_commands(t_master *master)
{
	int	original_stdin;
	int	original_stdout;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (master->cmd->fd_in != STDIN_FILENO)
	{
		dup2(master->cmd->fd_in, STDIN_FILENO);
		close(master->cmd->fd_in);
	}
	if (master->cmd->fd_out != STDOUT_FILENO)
	{
		dup2(master->cmd->fd_out, STDOUT_FILENO);
		close(master->cmd->fd_out);
	}
	status = check_internal_c(master);
	dup_and_close(original_stdin, original_stdout);
	return (status);
}
