/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_int_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:52:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:25:46 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_internal_c(t_cmd *cmd, char ***env, t_ctx *ctx,
	t_tok *tokens)
{
	t_tok	*current;

	current = tokens;
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd, cmd->argv[1], env, ctx));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd, env, ctx));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd, env));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*env, cmd));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv, ctx, tokens, cmd));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd->argv, current));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd());
	else
		return (127);
}

static int	intcmd_pipe_child(t_cmd *cmd, t_tok *start, t_tok *end, t_ctx *ctx)
{
	int	status ;

	if (setup_cmd_fds(cmd, start, end, ctx) != 0)
		exit(1);
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
	status = check_internal_c(cmd, &ctx->our_env, ctx, start);
	exit(status);
}

static int	intcmd_pipe(t_cmd *cmd, t_tok *start, t_tok *end, t_ctx *ctx)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	else if (pid == 0)
		intcmd_pipe_child(cmd, start, end, ctx);
	else
	{
		if (ctx->current_index > 0)
			close(ctx->pipe_fds[ctx->current_index - 1][0]);
		if (ctx->current_index < ctx->pipe_count)
			close(ctx->pipe_fds[ctx->current_index][1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}
	return (status);
}

static int	intcmd_no_pipe(t_cmd *cmd, t_tok *start, t_tok *end, t_ctx *ctx)
{
	int	fd_in;
	int	fd_out;
	int	status;

	status = 0;
	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (setup_redir(start, end, &cmd->fd_in, &cmd->fd_out, ctx) != 0)
	{
		restore_fds(fd_in, fd_out);
		return (1);
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
	status = check_internal_c(cmd, &ctx->our_env, ctx, start);
	restore_fds(fd_in, fd_out);
	return (status);
}

int	execute_internal_cmd(t_ctx *ctx, t_tok *start, t_tok *end)
{
	t_cmd	*cmd;
	int		status;

	status = 0;
	cmd = init_internal_cmd(start);
	if (!cmd)
		return (1);
	if (ctx->pipe_count != 0)
		status = intcmd_pipe(cmd, start, end, ctx);
	else
		status = intcmd_no_pipe(cmd, start, end, ctx);
	free_command(cmd);
	return (status);
}
