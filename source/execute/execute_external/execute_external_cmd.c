/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:46:35 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/30 18:03:08 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_external_c(char *cmd_path, char **args, int token_count)
{
	int	i;

	(void)token_count;
	free(cmd_path);
	i = 0;
	free(args);
}

int	execute_external_commands(t_master *master, int token_count)
{
	char	*cmd_path;
	char	**args;
	int		status;
	int		check_result;

	check_result = check_cmd_path(&cmd_path, master->token->cur, master->ctx);
	if (check_result != 0)
		return (check_result);
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd_path);
		return (1);
	}
	prep_args(master->token->cur, token_count, args, master->ctx);
	status = fork_and_execute(master, cmd_path, args);
	free_external_c(cmd_path, args, token_count);
	return (status);
}
