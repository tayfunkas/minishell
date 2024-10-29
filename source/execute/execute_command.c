/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:44:33 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/29 21:14:07 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_command_fds(t_command *cmd, t_token *start, t_token *end)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
}

int	execute_command(t_token *start, t_token *end, t_exec_context *ctx)
{
	t_command	cmd;
	int			status;
	int			fd_in;

	status = 0;
	if (!start)
		return (status);
	if (ft_strcmp(start->str, "<<") == 0)
	{
		fd_in = STDIN_FILENO;
		execute_redir_heredoc(start, &fd_in);
		start = start->next->next;
		if (start && start->type == CMD)
		{
			setup_command_fds(&cmd, start, end);
			cmd.fd_in = fd_in;
			status = execute_ext_or_int(start, end, &cmd, ctx);
			if (cmd.fd_out != STDOUT_FILENO)
				close(cmd.fd_out);
		}
		else
			close(fd_in);
	}
	else if (start->type == CMD)
	{
		setup_command_fds(&cmd, start, end);
		status = execute_ext_or_int(start, end, &cmd, ctx);
		if (cmd.fd_in != STDIN_FILENO)
			close(cmd.fd_in);
		if (cmd.fd_out != STDOUT_FILENO)
			close(cmd.fd_out);
	}
	return (status);
}

/*int	execute_command(t_token *start, t_token *end, t_exec_context *ctx)
{
	t_command	cmd;
	int			status;

	status = 0;
	if (start && start->type == CMD)
	{
		setup_command_fds(&cmd, start, end);
		status = execute_ext_or_int(start, end, &cmd, ctx);
		if (cmd.fd_in != STDIN_FILENO)
			close(cmd.fd_in);
		if (cmd.fd_out != STDOUT_FILENO)
			close(cmd.fd_out);
	}
	return (status);
}*/
