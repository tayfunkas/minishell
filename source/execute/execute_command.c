/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:44:33 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/24 14:51:03 by kyukang          ###   ########.fr       */
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

	status = 0;
	if (start && start->type == CMD)
	{
		setup_command_fds(&cmd, start, end);
		status = execute_ext_or_int(start, end, &cmd, ctx);
		check_signal_status();
		if (cmd.fd_in != STDIN_FILENO)
			close(cmd.fd_in);
		if (cmd.fd_out != STDOUT_FILENO)
			close(cmd.fd_out);
	}
	return (status);
}
