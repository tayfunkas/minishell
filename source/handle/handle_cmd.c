/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:06:11 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/03 17:10:42 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	is_external_cmd(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	int		result;

	if (ft_strcmp(cmd, "env") == 0)
		return (0);
	path_env = get_path_env(our_env);
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (0);
	result = check_cmd_in_paths(cmd, paths);
	free_split(paths);
	return (result);
}
 */

static int	is_internal_cmd(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	return (0);
}

static int	is_redir(char *cmd)
{
	if (ft_strcmp(cmd, "<") == 0 || ft_strcmp(cmd, "<<") == 0 || ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0)
		return (1);
	return (0);
}

int	handle_cmd(t_token *current, t_token *cmd_end, t_exec_context *ctx)
{
	int	status;
	int	fd_in;
	int	fd_out;

	status = 0;
	if (current == NULL || current->str == NULL)
	{
		if (ctx->syntax_error)
			return (2);
		else
			return (0);
	}
	if (is_internal_cmd(current->str))
		status = execute_internal_cmd(ctx, current, cmd_end, current);
	else if (is_redir(current->str))
	{
		fd_in = STDIN_FILENO;
		fd_out = STDOUT_FILENO;
		setup_redir(current, cmd_end, &fd_in, &fd_out);
	}
	else
		status = execute_external_cmd(ctx, current, cmd_end);
	return (status);
}
