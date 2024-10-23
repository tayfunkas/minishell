/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_internal_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:31:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/23 14:58:52 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc failed.\n");
		return (NULL);
	}
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->next = NULL;
	return (cmd);
}

static int	count_command_args(t_token *current)
{
	int	count;

	count = 0;
	while (current && (current->type == CMD || current->type == ARG))
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	allocate_command_argv(t_command *cmd, t_token *current)
{
	int		index;

	index = 0;
	while (current && (current->type == CMD || current->type == ARG))
	{
		cmd->argv[index] = ft_strdup(current->str);
		if (!cmd->argv[index])
		{
			perror("malloc failed.\n");
			return (0);
		}
		index++;
		current = current->next;
	}
	cmd->argv[index] = NULL;
	return (1);
}

t_command	*init_internal_command(t_token *current)
{
	t_command	*cmd;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	cmd->argc = count_command_args(current);
	cmd->argv = (char **)malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
	{
		perror("malloc failed.\n");
		free_command(cmd);
		return (NULL);
	}
	if (!allocate_command_argv(cmd, current))
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
