/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_int_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:54:29 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:30:57 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
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

static int	count_cmd_args(t_tok *current)
{
	int	count;

	count = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == INPUT || current->type == TRUNC
			|| current->type == APPEND)
		{
			if (current->next->next && current->next->next->type != PIPE)
				current = current->next->next;
			else
				break ;
		}
		if (current->type == CMD || current->type == ARG)
		{
			count++;
		}
		current = current->next;
	}
	return (count);
}

static int	if_type_is_redir(t_tok *current)
{
	if (current->type == INPUT || current->type == TRUNC
		|| current->type == APPEND)
	{
		if (current->next->next)
			current = current->next->next;
		else
			return (0);
	}
	return (1);
}

static int	allocate_command_argv(t_cmd *cmd, t_tok *current)
{
	int		index;

	index = 0;
	while (current && index < cmd->argc && current->type != PIPE)
	{
		if (!if_type_is_redir(current))
			break ;
		if (current->type == CMD || current->type == ARG)
		{
			cmd->argv[index] = ft_strdup(current->str);
			if (!cmd->argv[index])
			{
				perror("malloc failed.\n");
				return (0);
			}
			index++;
		}
		current = current->next;
	}
	cmd->argv[index] = NULL;
	return (1);
}

t_cmd	*init_internal_cmd(t_tok *current)
{
	t_cmd	*cmd;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	cmd->argc = count_cmd_args(current);
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
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	return (cmd);
}
