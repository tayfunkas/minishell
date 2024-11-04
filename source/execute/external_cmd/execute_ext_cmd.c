/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:15:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:10 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_external_c(char *cmd_path, char **args, int token_count)
{
	int	i;

	(void)token_count;
	free(cmd_path);
	i = 0;
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
	char	**args;

	token_count = count_toks_until(start, end);
	status = check_cmd_path(&cmd.cmd_path, start, ctx);
	if (status != 0)
		return (status);
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd.cmd_path);
		return (-1);
	}
	prep_args(start, token_count, args, ctx);
	status = fork_and_execute(&cmd, args, ctx, start, end);
	free_external_c(cmd.cmd_path, args, token_count);
	return (status);
}
