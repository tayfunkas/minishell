/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/30 14:56:18 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_pipe_redir(t_token *current)
{
	if (current->type == PIPE)
	{
		//handle_pipe(tokens, current->next);
		return ;
	}
	if (current->type == TRUNC || current->type == APPEND
		|| current->type == INPUT)
	{
		//handle_redirection(tokens, current->next->str, current->type);
		return ;
	}
}

static int is_external_command(char *cmd, char **our_env)
{
	char	*path_env = NULL;
	char	**paths;
	char	full_path[1024];
	int	i = 0;

	if (ft_strcmp(cmd, "env") == 0)
		return 0;
	while (our_env[i])
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break;
		}
		i++;
	}

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

void	handle_tokens(t_token *tokens, char **our_env)
{
	t_token		*current;
	t_command	*cmd;

	current = tokens;
	while (current != NULL)
	{
		is_pipe_redir(current);
		current = current->next;
	}
	current = tokens;
	if (current && current->type == CMD)
	{
		if (is_external_command(current->str, our_env))
		{
			printf("(external)\n");
			execute_external_command(current, count_tokens(current));
		}
		else if (is_internal_command(current->str))
		{
			cmd = init_internal_command(current, our_env);
			if (!cmd)
				return ;
			printf("(internal)\n");
			execute_internal_commands(cmd, &our_env);
			free_command(cmd);
		}
	}
}
