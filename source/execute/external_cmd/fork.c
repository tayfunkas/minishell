/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:31:30 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 15:40:37 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_child_process(t_cmd *cmd, char *cmd_path, char **args,
	t_ctx *ctx)
{
	int	exec_result;

	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
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
	exec_result = execve(cmd_path, args, ctx->our_env);
	if (ctx->our_env == NULL)
	{
		printf("env issue in child\n");
		return (88);
	}
	if (exec_result == -1)
		exit(0);
	return (0);
}

static void	child_signal_for_wait(void)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
}

static int	wait_child(pid_t pid, int *status, t_ctx *ctx)
{
	child_signal_for_wait();
	if (ctx->current_index > 0)
		close(ctx->pipe_fds[ctx->current_index - 1][0]);
	if (ctx->current_index < ctx->pipe_count)
		close(ctx->pipe_fds[ctx->current_index][1]);
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
		return (128 + WTERMSIG(*status));
	return (-1);
}

int	fork_and_execute(t_cmd *cmd, char **args, t_ctx *ctx, t_tok *start, t_tok *end)
{
	pid_t	pid;
	int		parent_in;
	int		parent_out;
	int		status;

	status = 0;
	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		if (setup_cmd_fds(cmd, start, end, ctx))
			exit(1);
		status = handle_child_process(cmd, cmd->cmd_path, args, ctx);
		exit(status);
	}
	else if (pid > 0)
		status = wait_child(pid, &status, ctx);
	else
		return (-1);
	restore_fds(parent_in, parent_out);
	setup_signal();
	return (status);
}

/*int	fork_and_execute(t_cmd *cmd, char **args, t_ctx *ctx, t_tok *start, t_tok *end)
{
	pid_t	pid;
	int		parent_in;
	int		parent_out;
	int		status;

	status = 0;
	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		if (setup_cmd_fds(cmd, start, end, ctx))
			exit(1);
		status = handle_child_process(cmd, cmd->cmd_path, args, ctx);
		exit(status);
	}
	else if (pid > 0)
	{
		child_signal_for_wait();
		if (ctx->current_index > 0)
			close(ctx->pipe_fds[ctx->current_index - 1][0]);
		if (ctx->current_index < ctx->pipe_count)
			close(ctx->pipe_fds[ctx->current_index][1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else
	{
		perror("fork");
		return (-1);
	}
	restore_fds(parent_in, parent_out);
	setup_signal();
	return (status);
}*/