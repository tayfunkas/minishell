/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:01:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 23:16:17 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_redir_trunc(t_tok *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		g_signal = 1;
		return (1);
	}
	return (0);
}

int	execute_redir_append(t_tok *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd_out == -1)
	{
		write(2, "minishell: ", 11);
		write(2, current->next->str, ft_strlen(current->next->str));
		write(2, ": Permission denied\n", 20);
		g_signal = 1;
		return (1);
	}
	return (0);
}

int	execute_redir_input(t_tok *current, int *fd_in)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = open(current->next->str, O_RDONLY);
	if (*fd_in == -1)
	{
		write(2, "minishell: ", 11);
		write(2, current->next->str, ft_strlen(current->next->str));
		write(2, ": No such file or directory\n", 28);
		g_signal = 1;
		return (1);
	}
	return (0);
}

/*void	execute_redir_heredoc(t_tok *current, int *fd_in, t_ctx *ctx)
{
	int		pipe_fd[2];
	char	*delims[100];
	int		delim_count;
	pid_t	pid;

	delim_count = 0;
	recursive_heredoc(current, delims, &delim_count);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		heredoc_input(pipe_fd, delims, delim_count, current, ctx);
	else
		heredoc_wait(pipe_fd, fd_in, pid);
}*/

/*int	setup_redir(t_tok *start, t_tok *end, t_cmd *cmd, t_ctx *ctx)
{
	t_tok	*current;
	int		status;

	current = start;
	while (current != end && current != NULL)
	{
		if (current->type == TRUNC)
			status = execute_redir_trunc(current, &cmd->fd_out);
		else if (current->type == APPEND)
			status = execute_redir_append(current, &cmd->fd_out);
		else if (current->type == INPUT)
			status = execute_redir_input(current, &cmd->fd_in);
		else if (current->type == HEREDOC)
			execute_redir_heredoc(current, &cmd->fd_in, ctx);
		if (status == 1)
			return (1);
		current = current->next;
	}
	return (0);
}*/

int	setup_redir(t_tok *start, t_tok *end, int *fd_in, int *fd_out, t_ctx *ctx, t_cmd *cmd)
{
	t_tok	*current;
	int		status;

	current = start;
	status = 0;
	while (current != end && current != NULL)
	{
		if (current->type == INPUT)
			status = execute_redir_input(current, fd_in);
		else if (current->type == TRUNC)
			status = execute_redir_trunc(current, fd_out);
		else if (current->type == APPEND)
			status = execute_redir_append(current, fd_out);
		else if (current->type == HEREDOC)
			execute_redir_heredoc(start, end, fd_in, ctx, cmd);
		if (status == 1)
			return (1);
		current = current->next;
	}
	return (0);
}
