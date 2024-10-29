/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:46:35 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/29 18:54:04 by tkasapog         ###   ########.fr       */
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

int	execute_external_commands(t_token *tokens, int token_count, 
			t_command *cmd, t_exec_context *ctx)
{
	char	*cmd_path;
	char	**args;
	int		status;
	int		check_result;

	check_result = check_cmd_path(&cmd_path, tokens, ctx);
	if (check_result != 0) 
		return (check_result);
	args = allocate_args(token_count);
	if (args == NULL) 
	{
		free(cmd_path);
		return (1);
	}
	prep_args(tokens, token_count, args, ctx);
	status = fork_and_execute(cmd, cmd_path, args, ctx);
	free_external_c(cmd_path, args, token_count);
	return (status);
}
