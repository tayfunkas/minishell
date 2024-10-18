/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:38:51 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/18 17:04:01 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_pipes(t_exec_context *ctx)
{
	if (ctx->current_index > 0)
		dup2(ctx->pipe_fds[ctx->current_index - 1][0], STDIN_FILENO);
	if (ctx->current_index < ctx->pipe_count)
		dup2(ctx->pipe_fds[ctx->current_index][1], STDOUT_FILENO);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
}

static void	handle_child_process(t_token *current, t_token *cmd_end,
			t_exec_context *ctx)
{
	int	status;
	
	setup_child_pipes(ctx);
	status = execute_command(current, cmd_end, ctx->our_env);
	exit(status);
}

int	handle_command(t_token *current, t_token *cmd_end, t_exec_context *ctx,
			pid_t *pids)
{
	int	status;
	
	status = 0;
	if (is_internal_command(current->str) && ctx->pipe_count == 0)
		status = execute_command(current, cmd_end, ctx->our_env);
	else
	{
		pids[ctx->current_index] = fork();
		if (pids[ctx->current_index] == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (pids[ctx->current_index] == 0)
			handle_child_process(current, cmd_end, ctx);
	}
	return (status);
}
