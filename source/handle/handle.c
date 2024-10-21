/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/21 16:45:12 by kyukang          ###   ########.fr       */
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
	return (status);
}

static int	reset_context_for_command(t_exec_context *ctx, t_token *tokens)
{
	int	pipe_count;

	pipe_count = count_pipes(tokens);
	ctx->pipe_count = pipe_count;
	ctx->current_index = 0;
	ctx->pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!ctx->pipe_fds)
		return (0);
	if (!create_pipes(ctx->pipe_fds, pipe_count))
	{
		free_pipe_fds(ctx->pipe_fds, pipe_count);
		return (0);
	}
	return (1);
}

int	handle_tokens(t_token *tokens, t_exec_context *ctx)
{
	pid_t	*pids;
	int		i;
	int		status;

	if (!reset_context_for_command(ctx, tokens))
		return (-1);
	pids = malloc(sizeof(pid_t) * (ctx->pipe_count + 1));
	if (!pids)
	{
		free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
		return (-1);
	}
	i = 0;
	while (i <= ctx->pipe_count)
		pids[i++] = 0;
	status = handle_tokens_loop(tokens, ctx, pids);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	status = wait_for_children(pids, ctx->pipe_count);
	free(pids);
	return (status);
}

/*
static t_exec_context	*init_context(t_token *tokens, char **our_env)
{
	t_exec_context	*ctx;
	int				pipe_count;

	pipe_count = count_pipes(tokens);
	ctx = malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	ctx->pipe_count = pipe_count;
	ctx->our_env = our_env;
	ctx->current_index = 0;
	ctx->pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!ctx->pipe_fds)
	{
		free_pipe_fds(ctx->pipe_fds, pipe_count);
		free(ctx);
		return (NULL);
	}
	if (!create_pipes(ctx->pipe_fds, pipe_count))
	{
		free_pipe_fds(ctx->pipe_fds, pipe_count);
		free(ctx);
		return (NULL);
	}
	return (ctx);
}

void	handle_tokens(t_token *tokens, char **our_env)
{
	t_exec_context	*ctx;
	pid_t			*pids;
	int				i;

	ctx = init_context(tokens, our_env);
	if (!ctx)
		return ;
	pids = malloc(sizeof(pid_t) * (ctx->pipe_count + 1));
	if (!pids)
	{
		free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
		free(ctx);
		return ;
	}
	i = 0;
	while (i <= ctx->pipe_count)
		pids[i++] = 0;
	handle_tokens_loop(tokens, ctx, pids);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	wait_for_children(pids, ctx->pipe_count);
	free(pids);
	free(ctx);
}*/

/*void	handle_tokens(t_token *tokens, char **our_env)
{
	t_exec_context	ctx;
	pid_t			*pids;
	int				pipe_count;
	int				i;

	pipe_count = count_pipes(tokens);
	ctx.pipe_count = pipe_count;
	ctx.our_env = our_env;
	ctx.current_index = 0;
	ctx.pipe_fds = malloc(sizeof(int *) * pipe_count);
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!ctx.pipe_fds || !pids)
	{
		free_pipe_fds(ctx.pipe_fds, pipe_count);
		free(pids);
		return ;
	}
	i = 0;
	while (i <= pipe_count)
		pids[i++] = 0;
	if (!create_pipes(ctx.pipe_fds, pipe_count))
	{
		free_pipe_fds(ctx.pipe_fds, pipe_count);
		free(pids);
		return ;
	}
	handle_tokens_loop(tokens, &ctx, pids);
	close_pipes(ctx.pipe_fds, pipe_count);
	free_pipe_fds(ctx.pipe_fds, pipe_count);
	wait_for_children(pids, pipe_count);
	free(pids);
}*/

/*void	handle_tokens(t_token *tokens, char **our_env)
{
	t_exec_context	ctx;
	pid_t			*pids;
	int				pipe_count;

	pipe_count = count_pipes(tokens);
	ctx.pipe_count = pipe_count;
	ctx.our_env = our_env;
	ctx.current_index = 0;
	ctx.pipe_fds = malloc(sizeof(int *) * pipe_count);
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!create_pipes(ctx.pipe_fds, pipe_count))
	{
		free_pipe_fds(ctx.pipe_fds, pipe_count);
		free(pids);
		return ;
	}
	handle_tokens_loop(tokens, &ctx, pids);
	close_pipes(ctx.pipe_fds, pipe_count);
	free_pipe_fds(ctx.pipe_fds, pipe_count);
	wait_for_children(pids, pipe_count);
	free(pids);
}
*/