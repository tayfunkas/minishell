/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:46:35 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 16:25:46 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_external_c(char *cmd_path, char **args, int token_count)
{
	int	i;

	free(cmd_path);
	i = 0;
	while (i < token_count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	execute_external_commands(t_token *tokens, int token_count,
			t_command *cmd)
{
	char		*cmd_path;
	char		**args;
	const char	*error_message;

	cmd_path = find_cmd_path(tokens->str, cmd->env);
	if (cmd_path == NULL)
	{
		error_message = "Command not found: ";
		write(STDERR_FILENO, error_message, ft_strlen(error_message));
		write(STDERR_FILENO, tokens->str, ft_strlen(tokens->str));
		write(STDERR_FILENO, "\n", 1);
		return ;
	}
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd_path);
		return ;
	}
	prep_args(tokens, token_count, args, cmd->env);
	fork_and_execute(cmd, cmd_path, args);
	free_external_c(cmd_path, args, token_count);
}
