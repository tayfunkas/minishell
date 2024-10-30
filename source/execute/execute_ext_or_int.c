/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_or_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:43:31 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/30 19:07:38 by kyukang          ###   ########.fr       */
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

int	execute_ext_or_int(t_master *master, t_token *end)
{
	int			arg_count;
	int			status;
	char		*cmd_path;

	cmd_path = NULL;
	if (is_internal_command(master->token->cur->str))
	{
		master->cmd->int_cmd = init_internal_command(master);
		if (!master->cmd->int_cmd)
			return (1);
		master->cmd->int_cmd->fd_in = master->cmd->fd_in;
		master->cmd->int_cmd->fd_out = master->cmd->fd_out;
		status = execute_internal_commands(master);
		//free_command(master->cmd->int_cmd);
		return (status);
	}
	arg_count = count_tokens_until(master->token->cur, end);
	status = execute_external_commands(master, arg_count);
	free(cmd_path);
	return (status);
}
