/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:45:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 16:34:57 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	errmsg_if_no_line(char *line, char *delimiter)
{
	if (!line)
	{
		printf("minishell: warning: here-document at line ");
		printf("delimited by end-of-file (wanted '%s')\n", delimiter);
	}
}

void	heredoc_wait(int *pipe_fd, int *fd_in, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	setup_signal();
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
}

void	recursive_heredoc(t_tok *current, char **delims, int *delim_count)
{
	if (current == NULL || current->type != HEREDOC)
		return ;
	delims[*delim_count] = current->next->str;
	(*delim_count)++;
}

void	heredoc_input(int *pipe_fd, char **delims, int delim_count, t_tok *current, t_ctx *ctx)
{
	char	*line;
	int		current_delim;

	current_delim = 0;
	signal(SIGINT, child_sigint_handler);
	close(pipe_fd[0]);
	while (current_delim < delim_count)
	{
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delims[current_delim]) == 0)
			{
				errmsg_if_no_line(line, delims[current_delim]);
				free(line);
				break ;
			}
			if (write(pipe_fd[1], line, ft_strlen(line)) == -1
				|| write(pipe_fd[1], "\n", 1) == -1)
			{
				perror("write to pipe");
				free(line);
				free_tokens(current);
				free_context(ctx);
				exit(EXIT_FAILURE);
			}
			free(line);
		}
		current_delim++;
	}
	close(pipe_fd[1]);
	free_tokens(current);
	free_context(ctx);
	exit(EXIT_SUCCESS);
}

void	execute_redir_heredoc(t_tok *current, int *fd_in, t_ctx *ctx)
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
}
