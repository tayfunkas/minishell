/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_with_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:20:51 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 02:01:49 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_heredoc_child(t_tok *start, t_ctx *ctx, char *delim,
	t_cmd *cmd)
{
	free_tokens(start);
	free_context(ctx);
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->argv)
		free(cmd->argv);
	free(delim);
}

void	handle_heredoc_child(t_heredoc *heredoc, t_ctx *ctx, t_tok *start,
	t_cmd *cmd)
{
	char	*line;

	close(heredoc->pipe_fd[0]);
	signal(SIGINT, child_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line_or_delim(heredoc->delim, line))
			break ;
		write(heredoc->pipe_fd[1], line, ft_strlen(line));
		write(heredoc->pipe_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc->pipe_fd[1]);
	free_heredoc_child(start, ctx, heredoc->delim, cmd);
	exit(EXIT_SUCCESS);
}

int	delim_or_pipe_issue(char *delim, int *pipe_fd)
{
	if (!delim)
		return (0);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(delim);
		return (0);
	}
	return (1);
}

static void	execute_redir_heredoc(t_tok *start, t_tok *end, t_ctx *ctx,
	t_cmd *cmd)
{
	pid_t		pid;
	t_heredoc	heredoc;

	heredoc.delim = NULL;
	collect_heredoc_delimiters(start, end, &heredoc.delim);
	if (!delim_or_pipe_issue(heredoc.delim, heredoc.pipe_fd))
		return ;
	pid = fork();
	if (pid == -1)
	{
		heredoc_pid_error(heredoc.pipe_fd, heredoc.delim);
		return ;
	}
	if (pid == 0)
		handle_heredoc_child(&heredoc, ctx, start, cmd);
	else
		handle_heredoc_parent(heredoc.pipe_fd, &cmd->fd_in, pid, heredoc.delim);
}

int	setup_redir(t_tok *start, t_tok *end, t_ctx *ctx, t_cmd *cmd)
{
	t_tok	*current;
	int		status;

	current = start;
	status = 0;
	while (current != end && current != NULL)
	{
		if (current->type == INPUT)
			status = execute_redir_input(current, &cmd->fd_in);
		else if (current->type == TRUNC)
			status = execute_redir_trunc(current, &cmd->fd_out);
		else if (current->type == APPEND)
			status = execute_redir_append(current, &cmd->fd_out);
		else if (current->type == HEREDOC)
			execute_redir_heredoc(start, end, ctx, cmd);
		if (status == 1)
			return (1);
		current = current->next;
	}
	return (0);
}
