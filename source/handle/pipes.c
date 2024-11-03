/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:03:39 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/03 17:10:09 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
	if (ctx->current_index > 0 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = ctx->pipe_fds[ctx->current_index - 1][0];
	if (ctx->current_index < ctx->pipe_count && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = ctx->pipe_fds[ctx->current_index][1];
}

void	setup_child_pipes(t_exec_context *ctx)
{
	if (ctx->current_index > 0)
	{
		if (dup2(ctx->pipe_fds[ctx->current_index - 1][0], STDIN_FILENO) == -1)
			perror("dup2 error on stdin");
		close(ctx->pipe_fds[ctx->current_index - 1][0]);
	}
	if (ctx->current_index < ctx->pipe_count)
	{
		if (dup2(ctx->pipe_fds[ctx->current_index][1], STDOUT_FILENO) == -1)
			perror("dup2 error on stdout");
		close(ctx->pipe_fds[ctx->current_index][1]);
	}
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
}

void	close_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	if (pipe_fds == NULL)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}

void	free_pipe_fds(int **pipe_fds, int pipe_count)
{
	int	i;

	if (!pipe_fds)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (pipe_fds[i])
			free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
}

int	create_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			return (0);
		}
		i++;
	}
	return (1);
}

int	count_pipes(t_token *tokens)
{
	int		pipe_count;
	t_token	*current;

	pipe_count = 0;
	current = tokens;
	while (current != NULL)
	{
		if (current->type == PIPE)
			pipe_count++;
		current = current->next;
	}
	return (pipe_count);
}
