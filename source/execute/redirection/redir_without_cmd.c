/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_without_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:33:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 06:50:18 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_heredoc_child(t_tok *start, t_ctx *ctx, char *delim)
{
	free_tokens(start);
	free_context(ctx);
	free(delim);
}

static void	handle_heredoc_child(int *pipe_fd, char *delim, t_ctx *ctx,
	t_tok *start)
{
	char	*line;

	close(pipe_fd[0]);
	signal(SIGINT, child_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line_or_delim(delim, line))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	free_heredoc_child(start, ctx, delim);
	exit(EXIT_SUCCESS);
}

static void	handle_heredoc_parent(int *pipe_fd, int *fd_in, pid_t pid, char *delim)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	free(delim);
	setup_signal();
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
}

static void	execute_redir_heredoc_only(t_tok *start, t_tok *end, int *fd_in,
	t_ctx *ctx)
{
	int		pipe_fd[2];
	char	*delim;
	pid_t	pid;

	delim = NULL;
	collect_heredoc_delimiters(start, end, &delim);
	if (!delim)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(delim);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		heredoc_pid_error(pipe_fd, delim);
		return ;
	}
	if (pid == 0)
		handle_heredoc_child(pipe_fd, delim, ctx, start);
	else
		handle_heredoc_parent(pipe_fd, fd_in, pid, delim);
}

int	setup_redir_only(t_tok *start, t_tok *end, t_ctx *ctx)
{
	t_tok	*current;
	int		status;
	int		fd_in;
	int		fd_out;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	current = start;
	status = 0;
	while (current != end && current != NULL)
	{
		if (current->type == INPUT)
			status = execute_redir_input(current, &fd_in);
		else if (current->type == TRUNC)
			status = execute_redir_trunc(current, &fd_out);
		else if (current->type == APPEND)
			status = execute_redir_append(current, &fd_out);
		else if (current->type == HEREDOC)
			execute_redir_heredoc_only(start, end, &fd_in, ctx);
		if (status == 1)
			return (1);
		current = current->next;
	}
	return (0);
}
