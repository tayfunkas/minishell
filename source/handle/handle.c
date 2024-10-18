/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/18 18:35:00 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_children(pid_t *pids, int pipe_count)
{
	int	i;

	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

t_token	*find_cmd_end(t_token *current)
{
	t_token	*cmd_end;

	cmd_end = current;
	while (cmd_end && cmd_end->type != PIPE)
		cmd_end = cmd_end->next;
	return (cmd_end);
}

int	handle_tokens_loop(t_token *tokens, t_exec_context *ctxt, pid_t *pids)
{
	t_token	*current;
	t_token	*cmd_end;
	int		status;

	status = 0;
	current = tokens;
	while (ctxt->current_index <= ctxt->pipe_count)
	{
		cmd_end = find_cmd_end(current);
		status = handle_command(current, cmd_end, ctxt, pids);
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		ctxt->current_index++;
	}
	return (status);
}

static	t_exec_context	*init_context(t_token *tokens, char **our_env)
{
	t_exec_context	*ctx;
	int				pipe_count;

	pipe_count = count_pipes(tokens);
	ctx = malloc(sizeof(t_exec_context));
	if (!ctx)
		return (NULL);
	ctx->pipe_count = pipe_count;
	ctx->current_index = 0;
	ctx->our_env = our_env; 
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

int	handle_tokens(t_token *tokens, char **our_env)
{
	t_exec_context	*ctx;
	pid_t			*pids;
	int				i;
	int				status;

	ctx = init_context(tokens, our_env);
	if (!ctx)
		return (-1);
	pids = malloc(sizeof(pid_t) * (ctx->pipe_count + 1));
	if (!pids)
	{
		free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
		free(ctx);
		return (-1);
	}
	i = 0;
	while (i <= ctx->pipe_count)
		pids[i++] = 0;
	status = handle_tokens_loop(tokens, ctx, pids);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
	free_pipe_fds(ctx->pipe_fds, ctx->pipe_count);
	wait_for_children(pids, ctx->pipe_count);
	free(pids);
	free(ctx);
	return (status);
}

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
