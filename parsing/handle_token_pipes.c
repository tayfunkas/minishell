/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:59:10 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 13:01:22 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe_fds(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
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

void	close_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}

void	setup_child_pipes(t_exec_context *ctx)
{
	if (ctx->current_index > 0)
		dup2(ctx->pipe_fds[ctx->current_index - 1][0], STDIN_FILENO);
	if (ctx->current_index < ctx->pipe_count)
		dup2(ctx->pipe_fds[ctx->current_index][1], STDOUT_FILENO);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
}

t_token	*find_cmd_end(t_token *current)
{
	t_token	*cmd_end;

	cmd_end = current;
	while (cmd_end && cmd_end->type != PIPE)
		cmd_end = cmd_end->next;
	return (cmd_end);
}
