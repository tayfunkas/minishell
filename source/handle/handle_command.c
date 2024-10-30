/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:38:51 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/30 19:27:12 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_pipes(t_exec_context *ctx)
{
	if (ctx->current_index > 0)
		dup2(ctx->pipe_fds[ctx->current_index - 1][0], STDIN_FILENO);
	if (ctx->current_index < ctx->pipe_count)
		dup2(ctx->pipe_fds[ctx->current_index][1], STDOUT_FILENO);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
}

static void	handle_child_process(t_master *master, t_token *cmd_end)
{
	int	status;

	setup_child_pipes(master->ctx);
	status = execute_command(master, cmd_end);
	free_tokens(master->token->cur);
	free_context(master->ctx);
	exit(status);
}

int	handle_command(t_master *master, t_token *cmd_end)
{
	int	status;
	int	fd_in;

	fd_in = STDIN_FILENO;
	status = 0;
	if (master->token->cur == NULL || master->token->cur->str == NULL)
	{
		if (master->ctx->syntax_error)
			return (2);
		else
			return (0);
	}
	status = handle_initial_redir(master, &fd_in);
	if (status != 0)
		return (status);
	if (master->token->cur == NULL || master->token->cur->str == NULL)
		return (0);
	if (is_internal_command(master->token->cur->str) && master->ctx->pipe_count == 0)
		status = execute_command(master, cmd_end);
	else
	{
		master->ctx->pid[master->ctx->current_index] = fork();
		if (master->ctx->pid[master->ctx->current_index] == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (master->ctx->pid[master->ctx->current_index] == 0)
			handle_child_process(master, cmd_end);
	}
	return (status);
}
