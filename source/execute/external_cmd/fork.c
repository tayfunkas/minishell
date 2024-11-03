/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:31:30 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/02 20:44:12 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_child_process(t_command *cmd, char *cmd_path, char **args,
	t_exec_context *ctx)
{
	int	exec_result;

	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
	//setup_child_pipes(ctx);
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
	//duplicate_fds(cmd->fd_in, cmd->fd_out);
	exec_result = execve(cmd_path, args, ctx->our_env);
	if (ctx->our_env == NULL)
	{
		printf("env issue in child\n");
		return (88);
	}
	if (exec_result == -1)
	{
		//perror("execve");
		exit(0);
	}
	return (0);
}

static void	child_signal_for_wait(void)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
}

int	fork_and_execute(t_command *cmd, char *cmd_path, char **args,
	t_exec_context *ctx, t_token *start, t_token *end)
{
	pid_t	pid;
	int		parent_in;
	int		parent_out;
	int		status = 0;

	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		setup_cmd_fds(cmd, start, end, ctx);
		//printf("Child process - fd_in: %d, fd_out: %d\n", cmd->fd_in, cmd->fd_out);
		status = handle_child_process(cmd, cmd_path, args, ctx);
		exit(status);
	}
	else if (pid > 0)
	{
		child_signal_for_wait();
		check_fds(cmd);
		if (ctx->current_index > 0)
			close(ctx->pipe_fds[ctx->current_index - 1][0]);
		if (ctx->current_index < ctx->pipe_count)
			close(ctx->pipe_fds[ctx->current_index][1]);
		waitpid(pid, &status, 0);
		//restore_fds(parent_in, parent_out);
		//setup_signal();
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
}
