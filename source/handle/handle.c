/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:00:40 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 18:11:24 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_context(t_exec_context *ctx, t_token *tokens)
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

static t_token	*find_cmd_end(t_token *current)
{
	t_token	*cmd_end;

	cmd_end = current;
	while (cmd_end && cmd_end->type != PIPE)
		cmd_end = cmd_end->next;
	return (cmd_end);
}

static int	handle_tokens_loop(t_token *tokens, t_exec_context *ctx)
{
	t_token	*current;
	t_token	*cmd_end;
	int		status;

	status = 0;
	current = tokens;
	while (ctx->current_index <= ctx->pipe_count)
	{
		cmd_end = find_cmd_end(current);
		status = handle_cmd(current, cmd_end, ctx);
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		ctx->current_index++;
	}
	free_tokens(tokens);
	return (status);
}

int	handle_tokens(t_token *tokens, t_exec_context *ctx)
{
	int		status;

	if (!init_context(ctx, tokens))
		return (-1);
	status = handle_tokens_loop(tokens, ctx);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	ctx->pipe_fds = NULL;
	return (status);
}
