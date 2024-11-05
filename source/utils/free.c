/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:42 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 03:15:09 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_environment(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_context(t_ctx *ctx)
{
	int	i;

	if (ctx == NULL)
		return ;
	if (ctx->pipe_fds != NULL)
	{
		i = 0;
		while (i < ctx->pipe_count)
		{
			if (ctx->pipe_fds[i] != NULL)
			{
				free(ctx->pipe_fds[i]);
				ctx->pipe_fds[i] = NULL;
			}
			i++;
		}
		free(ctx->pipe_fds);
		ctx->pipe_fds = NULL;
	}
	if (ctx->our_env != NULL)
	{
		free_environment(ctx->our_env);
		ctx->our_env = NULL;
	}
}

void	free_command_list(t_cmd *head)
{
	t_cmd	*current;
	t_cmd	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

void	free_command(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	if (cmd->argv)
	{
		while (i < cmd->argc)
		{
			if (cmd->argv[i])
			{
				free(cmd->argv[i]);
				cmd->argv[i] = NULL;
			}
			i++;
		}
		free(cmd->argv);
		cmd->argv = NULL;
	}
	free(cmd);
}

void	free_tokens(t_tok *tokens)
{
	t_tok	*current;
	t_tok	*next;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		free(current);
		current = next;
	}
}
