/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:44:33 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/16 14:49:12 by tkasapog         ###   ########.fr       */
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

/*void	execute_command(t_token *start, t_token *end, char **our_env)
{
	int	fd_in = STDIN_FILENO;
	int	fd_out = STDOUT_FILENO;
	int	arg_count;

	setup_redir(start, end, &fd_in, &fd_out);
	if (start && start->type == CMD)
	{
		if (is_external_command(start->str, our_env))
		{
			arg_count = count_tokens_until(start, end);
			execute_external_command(start, arg_count, our_env, fd_in, fd_out);
		}
		else if (is_internal_command(start->str))
		{
			t_command *cmd = init_internal_command(start, our_env);
			if (!cmd)
				return;
			cmd->fd_in = fd_in;
			cmd->fd_out = fd_out;
			execute_internal_commands(cmd, &our_env);
			free_command(cmd);
		}
	}
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
}*/
