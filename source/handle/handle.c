/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/31 19:44:56 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_for_children(pid_t *pids, int pipe_count)
{
	int	i;
	int	last_status;
	int	status;

	i = 0;
	last_status = 0;
	while (i <= pipe_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

static t_token	*find_cmd_end(t_token *current)
{
	t_token	*cmd_end;

	cmd_end = current;
	while (cmd_end && cmd_end->type != PIPE)
		cmd_end = cmd_end->next;
	return (cmd_end);
}

static int	handle_tokens_loop(t_token *tokens, t_exec_context *ctx,
		pid_t *pids)
{
	t_token	*current;
	t_token	*cmd_end;
	int		status;

	status = 0;
	current = tokens;
	while (ctx->current_index <= ctx->pipe_count)
	{
		cmd_end = find_cmd_end(current);
		status = handle_command(current, cmd_end, ctx, pids);
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		ctx->current_index++;
	}
	printf("free_tokens\n");
	free_tokens(tokens);
	return (status);
}

static int	reset_context_for_command(t_exec_context *ctx, t_token *tokens)
{
	int	pipe_count;

	pipe_count = count_pipes(tokens);
	ctx->pipe_count = pipe_count;
	ctx->current_index = 0;
	if (pipe_count == 0)
	{
		ctx->pipe_fds = NULL;
		return (1);
	}
	ctx->pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!ctx->pipe_fds)
		return (0);
	if (!create_pipes(ctx->pipe_fds, pipe_count))
	{
		free_pipe_fds(ctx->pipe_fds, pipe_count);
		ctx->pipe_fds = NULL;
		return (0);
	}
	return (1);
}

int	handle_tokens(t_token *tokens, t_exec_context *ctx)
{
	int		status;
	int		child_status;

	ctx->child_pid = NULL;
	if (!reset_context_for_command(ctx, tokens))
		return (-1);
	ctx->child_pid = ft_calloc(ctx->pipe_count + 1, sizeof(pid_t));
	if (!ctx->child_pid)
	{
		free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
		ctx->pipe_fds = NULL;
		return (-1);
	}
	status = handle_tokens_loop(tokens, ctx, ctx->child_pid);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	ctx->pipe_fds = NULL;
	child_status = wait_for_children(ctx->child_pid, ctx->pipe_count);
	free(ctx->child_pid);
	if (child_status != 0)
		status = child_status;
	return (status);
}

/*int	handle_tokens(t_token *tokens, t_exec_context *ctx)
{
	pid_t	*pids;
	int		status;
	int		child_status;

	pids = NULL;
	if (!reset_context_for_command(ctx, tokens))
		return (-1);
	pids = ft_calloc(ctx->pipe_count + 1, sizeof(pid_t));
	if (!pids)
	{
		free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
		ctx->pipe_fds = NULL;
		return (-1);
	}
	status = handle_tokens_loop(tokens, ctx, pids);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	ctx->pipe_fds = NULL;
	child_status = wait_for_children(pids, ctx->pipe_count);
	free(pids);
	if (child_status != 0)
		status = child_status;
	return (status);
}
*/