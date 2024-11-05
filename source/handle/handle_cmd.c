/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:06:11 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 00:41:22 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_cmd_fds(t_cmd *cmd, t_tok *start, t_tok *end, t_ctx *ctx)
{
	int	status;

	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	status = setup_redir(start, end, &cmd->fd_in, &cmd->fd_out, ctx, cmd);
	if (status == 1)
		return (status);
	if (ctx->current_index > 0 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = ctx->pipe_fds[ctx->current_index - 1][0];
	if (ctx->current_index < ctx->pipe_count && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = ctx->pipe_fds[ctx->current_index][1];
	return (status);
}

static int	is_internal_cmd(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	return (0);
}

static int	is_redir(char *cmd)
{
	if (ft_strcmp(cmd, "<") == 0 || ft_strcmp(cmd, "<<") == 0
		|| ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0)
		return (1);
	return (0);
}

int	handle_cmd(t_tok *current, t_tok *cmd_end, t_ctx *ctx)
{
	int		status;

	status = 0;
	if (current == NULL || current->str == NULL)
	{
		if (ctx->syntax_error)
			return (2);
		else
			return (0);
	}
	if (is_internal_cmd(current->str))
		status = execute_internal_cmd(ctx, current, cmd_end);
	else if (is_redir(current->str))
		status = setup_redir_only(current, cmd_end, ctx);
	else
		status = execute_external_cmd(ctx, current, cmd_end);
	return (status);
}
