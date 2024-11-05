/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:45:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 23:38:45 by kyukang          ###   ########.fr       */
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
			printf("Delimiter added: %s\n", *delim);
			current = current->next->next;
			return ;
		}
		current = current->next;
	}
	*delim = NULL;
}

static void	handle_heredoc_child(int *pipe_fd, char *delim, t_ctx *ctx, t_tok *start, t_cmd *cmd)
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
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->argv)
		free(cmd->argv);
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

void	execute_redir_heredoc(t_tok *start, t_tok *end, int *fd_in, t_ctx *ctx, t_cmd *cmd)
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
		handle_heredoc_child(pipe_fd, delim, ctx, start, cmd);
	else
	{
		handle_heredoc_parent(pipe_fd, fd_in, pid);
		free(delim);
	}
}
