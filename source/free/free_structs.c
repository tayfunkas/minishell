/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:13:46 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/29 21:06:54 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_context(t_exec_context *ctx)
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

void	free_command_list(t_command *head)
{
	t_command	*current;
	t_command	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
