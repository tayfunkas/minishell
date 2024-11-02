/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:03:39 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/02 19:54:20 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	setup_child_pipes(t_exec_context *ctx)
{
	if (ctx->current_index > 0)
		dup2(ctx->pipe_fds[ctx->current_index - 1][0], STDIN_FILENO);
	if (ctx->current_index < ctx->pipe_count)
		dup2(ctx->pipe_fds[ctx->current_index][1], STDOUT_FILENO);
	close_pipes(ctx->pipe_fds, ctx->pipe_count);
}*/

/* void	setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;

	if (ctx->current_index > 0) // 이전 명령어가 있으면 파이프의 읽기 끝을 표준 입력으로 설정
		cmd->fd_in = ctx->pipe_fds[ctx->current_index - 1][0];
	if (ctx->current_index < ctx->pipe_count) // 다음 명령어가 있으면 파이프의 쓰기 끝을 표준 출력으로 설정
		cmd->fd_out = ctx->pipe_fds[ctx->current_index][1];
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
} */

void	setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
	//printf("After redir - fd_in: %d, fd_out: %d\n", cmd->fd_in, cmd->fd_out);
	if (ctx->current_index > 0 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = ctx->pipe_fds[ctx->current_index - 1][0];
	if (ctx->current_index < ctx->pipe_count && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = ctx->pipe_fds[ctx->current_index][1];
	//printf("After pipe setup - fd_in: %d, fd_out: %d\n", cmd->fd_in, cmd->fd_out);
}

void setup_child_pipes(t_exec_context *ctx)
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

/*void	close_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}*/

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
