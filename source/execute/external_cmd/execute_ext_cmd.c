/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ext_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:15:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 19:07:48 by kyukang          ###   ########.fr       */
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

static void	setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	(void)start;
	(void)end;
	(void)ctx;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
}

int	execute_external_cmd(t_exec_context *ctx, t_token *start, t_token *end)
{
	t_command	cmd;
	int			status;
	int			token_count;
	int			check_result;
	char		**args;
	char		*cmd_path;

	setup_cmd_fds(&cmd, start, end, ctx);
	//if (cmd.fd_in != STDIN_FILENO)
	//	close(cmd.fd_in);
	//if (cmd.fd_out != STDOUT_FILENO)
	//	close(cmd.fd_out);
	token_count = count_tokens_until(start, end);
	check_result = check_cmd_path(&cmd_path, start, ctx);
	if (check_result != 0)
		return (check_result);
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd_path);
		return (1);
	}
	prep_args(start, token_count, args, ctx);
	status = fork_and_execute(&cmd, cmd_path, args, ctx);
	free_external_c(cmd_path, args, token_count);
	return (status);
}
