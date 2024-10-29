/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute_extcmd.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:44:48 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/29 18:31:27 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_child_process(t_command *cmd, char *cmd_path, char **args,
	t_exec_context *ctx)
{
	int	exec_result;

	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
	duplicate_fds(cmd->fd_in, cmd->fd_out);
	exec_result = execve(cmd_path, args, ctx->our_env);
	if (ctx->our_env == NULL)
	{
		printf("env issue in child\n");
		return (88);
	}
	if (exec_result == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	return (0);
}

static void	child_signal_for_wait(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, child_sigquit_handler);
}

int	fork_and_execute(t_command *cmd, char *cmd_path, char **args,
	t_exec_context *ctx)
{
	pid_t	pid;
	int		parent_in;
	int		parent_out;
	int		status;

	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
		handle_child_process(cmd, cmd_path, args, ctx);
	else if (pid > 0)
	{
		child_signal_for_wait();
		check_fds(cmd);
		waitpid(pid, &status, 0);
		restore_fds(parent_in, parent_out);
		setup_signal();
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else
		perror("fork");
	return (1);
}
