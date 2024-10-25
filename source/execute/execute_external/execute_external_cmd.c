/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:46:35 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/24 19:16:41 by tkasapog         ###   ########.fr       */
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

/*static void	free_external_c(char *cmd_path, char **args, int token_count)
{
	int	i;

	(void)token_count;
	free(cmd_path);
	i = 0;
	while (i < token_count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}*/

/*int	execute_external_commands(t_token *tokens, int token_count,
			t_command *cmd, t_exec_context *ctx)
{
	char		*cmd_path;
	char		**args;
	const char	*error_message;
	int			status;

	status = 0;
	cmd_path = find_cmd_path(tokens->str, ctx->our_env);
	if (cmd_path == NULL)
	{
		error_message = "Command not found: 2";
		write(STDERR_FILENO, error_message, ft_strlen(error_message));
		write(STDERR_FILENO, tokens->str, ft_strlen(tokens->str));
		write(STDERR_FILENO, "\n", 1);
		return (127);
	}
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd_path);
		return (1);
	}
	prep_args(tokens, token_count, args, ctx);
	status = fork_and_execute(cmd, cmd_path, args);
	free_external_c(cmd_path, args, token_count);
	return (status);
}*/

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
