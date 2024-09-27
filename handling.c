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

/*static int	is_external_command(char *cmd)
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
}*/

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

void	handle_tokens(t_token *tokens, char **envp)
{
	t_token	*current;
	t_command	*cmd; // New command list to hold commands

	current = tokens;
	cmd = NULL;
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
	current = tokens;
	if (current && current->type == CMD)
	{
		cmd = (t_command *)malloc(sizeof(t_command));
		if (!cmd)
		{
			perror("malloc failied");
			return ;
		}
		cmd->argv = NULL;
		cmd->argc = 0;
		cmd->fd_in = 0;
		cmd->fd_out = 1;
		cmd->env = envp;
		cmd->next = NULL;
		t_token	*arg_token = current;
		while (arg_token && (arg_token->type == CMD || arg_token->type == ARG))
		{
			cmd->argc++;
			arg_token = arg_token->next;
		}
		cmd->argv = (char **)malloc((cmd->argc + 1) * sizeof(char *));
		if (!cmd->argv)
		{
			perror("malloc failed");
			free(cmd);
			return ;
		}
		int index = 0;
		arg_token = current;
		while (arg_token && (arg_token->type == CMD || arg_token->type == ARG))
		{
			cmd->argv[index] = ft_strdup(arg_token->str);
			if (!cmd->argv[index])
			{
				perror("strdup failed");
				free(cmd);
				return;
			}
		index++;
		arg_token = arg_token->next;
		}
		cmd->argv[index] = NULL;
		if (is_internal_command(cmd->argv[0]))
        		execute_internal_commands(cmd);
		int	l = 0;
		while (l < cmd->argc)
		{
			free(cmd->argv[l]);
			l++;
		}
		free(cmd->argv);
		free(cmd);
	}
}

