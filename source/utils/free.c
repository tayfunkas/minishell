/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:42 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:27:35 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_ctx *ctx, t_tok *tokens, t_cmd *cmdlist)
{
	free_context(ctx);
	free_tokens(tokens);
	free_command_list(cmdlist);
}

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

void	free_expand(t_expand *exp)
{
	if (exp == NULL)
		return ;
	if (exp->result != NULL)
		free(exp->result);
	if (exp->new_result != NULL)
		free(exp->new_result);
	if (exp->env_var != NULL)
		free(exp->env_var);
	exp->result = NULL;
	exp->new_result = NULL;
	exp->env_var = NULL;
	exp->env_value = NULL;
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
		free(current->str);
		free(current);
		current = next;
	}
}

void	free_command(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->argc)
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
	free(cmd);
}

void	free_split(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
