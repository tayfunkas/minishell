/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute_extcmd.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:44:48 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 16:46:50 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

void	restore_fds(int parent_in, int parent_out)
{
	dup2(parent_in, STDIN_FILENO);
	dup2(parent_out, STDOUT_FILENO);
	close(parent_in);
	close(parent_out);
}

int	handle_child_process(char *cmd_path, char **args)
{
	int	exec_result;

	exec_result = execve(cmd_path, args, NULL);
	if (exec_result == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	return (0);
}

int	fork_and_execute(t_command *cmd, char *cmd_path, char **args)
{
	pid_t	pid;
	int		parent_in;
	int		parent_out;
	int		status;

	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		duplicate_fds(cmd->fd_in, cmd->fd_out);
		handle_child_process(cmd_path, args);
	}
	else if (pid > 0)
	{
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		waitpid(pid, &status, 0);
		restore_fds(parent_in, parent_out);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
	return (1);
}
