/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_or_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:43:31 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/21 17:46:36 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens_until(t_token *start, t_token *end)
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

int	execute_ext_or_int(t_token *start, t_token *end, t_command *cmd)
{
	int			arg_count;
	t_command	*internal_cmd;
	int			status;
	char		*cmd_path;

	cmd_path = NULL;
	if (is_internal_command(start->str))
	{
		internal_cmd = init_internal_command(start, cmd->env);
		if (!internal_cmd)
			return (1);
		internal_cmd->fd_in = cmd->fd_in;
		internal_cmd->fd_out = cmd->fd_out;
		status = execute_internal_commands(internal_cmd, &cmd->env);
		free_command(internal_cmd);
		return (status);
	}
	status = check_cmd_path(cmd_path, cmd, start);
	arg_count = count_tokens_until(start, end);
	status = execute_external_commands(start, arg_count, cmd);
	free(cmd_path);
	return (status);
}
