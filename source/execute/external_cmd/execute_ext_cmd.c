/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:15:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 02:16:25 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_external_c(char *cmd_path, char **args, int token_count)
{
	(void)token_count;
	if (cmd_path)
		free(cmd_path);
	if (args)
		free(args);
}

static int	count_toks_until(t_tok *start, t_tok *end)
{
	int		count;
	t_tok	*current;

	count = 0;
	current = start;
	while (current != end && current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	execute_external_cmd(t_ctx *ctx, t_tok *start, t_tok *end)
{
	t_cmd	cmd;
	int		status;
	int		token_count;

	token_count = count_toks_until(start, end);
	status = check_cmd_path(&cmd.cmd_path, start, ctx);
	if (status != 0)
		return (status);
	cmd.argv = allocate_args(token_count);
	if (cmd.argv == NULL)
	{
		free(cmd.cmd_path);
		return (-1);
	}
	prep_args(start, token_count, cmd.argv, ctx);
	status = fork_and_execute(&cmd, ctx, start, end);
	free_external_c(cmd.cmd_path, cmd.argv, token_count);
	return (status);
}
