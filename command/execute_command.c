/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:44:33 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 14:02:26 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_command_fds(t_command *cmd, t_token *start, t_token *end)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
}

static void	handle_ext_or_int(t_token *start, t_token *end, t_command *cmd)
{
	int			arg_count;
	t_command	*internal_cmd;

	if (is_external_command(start->str, cmd->env))
	{
		arg_count = count_tokens_until(start, end);
		execute_external_c(start, arg_count, cmd);
	}
	else if (is_internal_command(start->str))
	{
		internal_cmd = init_internal_command(start, cmd->env);
		if (!internal_cmd)
			return ;
		internal_cmd->fd_in = cmd->fd_in;
		internal_cmd->fd_out = cmd ->fd_out;
		execute_internal_commands(internal_cmd, &cmd->env);
		free_command(internal_cmd);
	}
}

void	execute_command(t_token *start, t_token *end, char **our_env)
{
	t_command	cmd;

	cmd.env = our_env;
	if (start && start->type == CMD)
	{
		setup_command_fds(&cmd, start, end);
		handle_ext_or_int(start, end, &cmd);
		if (cmd.fd_in != STDIN_FILENO)
			close(cmd.fd_in);
		if (cmd.fd_out != STDOUT_FILENO)
			close(cmd.fd_out);
	}
}

int	count_tokens_until(t_token *start, t_token *end)
{
	int		count;
	t_token	*current;

	count = 0;
	current = start;
	while (current != end && current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}
