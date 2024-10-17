/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 12:54:47 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_token *current, t_token *cmd_end,
			t_exec_context *ctx)
{
	setup_child_pipes(ctx);
	execute_command(current, cmd_end, ctx->our_env);
	exit(EXIT_SUCCESS);
}

static void	wait_for_children(pid_t *pids, int pipe_count)
{
	int	i;

	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

static void	handle_command(t_token *current, t_token *cmd_end,
			t_exec_context *ctx, pid_t *pids)
{
	if (is_internal_command(current->str) && ctx->pipe_count == 0)
		execute_command(current, cmd_end, ctx->our_env);
	else
	{
		pids[ctx->current_index] = fork();
		if (pids[ctx->current_index] == -1)
		{
			perror("fork");
			return ;
		}
		else if (pids[ctx->current_index] == 0)
			handle_child_process(current, cmd_end, ctx);
	}
}

static void	handle_tokens_loop(t_token *tokens, t_exec_context *ctx, 
			pid_t *pids)
{
	t_token	*current;
	t_token	*cmd_end;

	current = tokens;
	while (ctx->current_index <= ctx->pipe_count)
	{
		cmd_end = find_cmd_end(current);
		handle_command(current, cmd_end, ctx, pids);
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		ctx->current_index++;
	}
}

void	handle_tokens(t_token *tokens, char **our_env)
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
