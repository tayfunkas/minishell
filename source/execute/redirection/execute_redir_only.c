/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_only.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:33:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 00:06:10 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static void	free_delimiters(char **delims, int delim_count)
{
	int	i;

	i = 0;
	while (i < delim_count)
	{
		free(delims[i]);
		i++;
	}
} */

static void	collect_heredoc_delimiters(t_tok *start, t_tok *end, char **delim)
{
	static t_tok	*current = NULL;

	if (current == NULL || current == end)
		current = start;
	while (current != end && current != NULL)
	{
		if (current->type == HEREDOC && current->next)
		{
			*delim = ft_strdup(current->next->str);
			if (*delim == NULL)
			{
				perror("Memory allocation failed");
				return ;
			}
			current = current->next->next;
			return ;
		}
		current = current->next;
	}
	*delim = NULL;
}

static void	handle_heredoc_child(int *pipe_fd, char *delim, t_ctx *ctx, t_tok *start)
{
	char	*line;

	close(pipe_fd[0]);
	signal(SIGINT, child_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", delim);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	free_tokens(start);
	free_context(ctx);
	free(delim);
	exit(EXIT_SUCCESS);
}

static void	handle_heredoc_parent(int *pipe_fd, int *fd_in, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
	else
		g_signal = WEXITSTATUS(status);
}

static void	execute_redir_heredoc_only(t_tok *start, t_tok *end, int *fd_in, t_ctx *ctx)
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
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		free(delim);
		return ;
	}
	if (pid == 0)
		handle_heredoc_child(pipe_fd, delim, ctx, start);
	else
	{
		handle_heredoc_parent(pipe_fd, fd_in, pid);
		free(delim);
	}
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
