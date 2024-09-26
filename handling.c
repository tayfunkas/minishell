/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:41:07 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/25 19:07:02 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//handle_pipe(), handle_redirection could be added in this file

static int	is_external_command(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (1);
		}
		i++;
	}
	free_split(paths);
	return (0);
}

static int	is_internal_command(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	handle_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current != NULL)
	{
		if (current->type == PIPE)
		{
			//handle_pipe(tokens, current->next);
			return;
		}
		if (current->type == TRUNC || current->type == APPEND || current->type == INPUT)
		{
			//handle_redirection(tokens, current->next->str, current->type);
			return;
		}
		current = current->next;
	}
	if (tokens->type == CMD)
	{
		if (is_external_command(tokens->str))
		{
			execute_external_command(tokens, count_tokens(tokens));
		//	printf("You entered external command.\n");
			return ;
		}
		else if (is_internal_command(tokens->str))
		{
		//	execute_internal_command(tokens);
			printf("You entered internal command.\n");
			return ;
		}
	}
}
