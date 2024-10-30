/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:18:40 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/30 17:39:37 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_result_buffer(t_expand *exp, char *static_buffer)
{
	exp->new_result = malloc(exp->result_size * 2);
	if (!exp->new_result)
		return (-1);
	ft_memcpy(exp->new_result, exp->result, exp->result_idx);
	if (exp->result != static_buffer)
		free(exp->result);
	exp->result = exp->new_result;
	exp->result_size *= 2;
	return (0);
}

static int	process_token_loop(char *token, t_expand *exp, t_exec_context *ctx)
{
	static char	static_buffer[1024];

	while (token[exp->token_idx])
	{
		if (handle_quotes(token, exp))
			continue ;
		if (token[exp->token_idx] == '$' && !exp->in_single_quote)
		{
			if (handle_env_var_exp(token, exp, ctx) == -1)
				return (-1);
		}
		else
			exp->result[exp->result_idx++] = token[exp->token_idx++];
		if (exp->result_idx >= (int)exp->result_size - 1)
		{
			if (expand_result_buffer(exp, static_buffer) == -1)
				return (-1);
		}
	}
	exp->result[exp->result_idx] = '\0';
	return (0);
}

char	*expand_var(char *token, t_exec_context *ctx)
{
	static char	static_buffer[1024];
	t_expand	exp;
	char		*final_result;

	exp.result = static_buffer;
	exp.result_size = 1024;
	exp.in_single_quote = 0;
	exp.in_double_quote = 0;
	exp.token_idx = 0;
	exp.result_idx = 0;
	if (process_token_loop(token, &exp, ctx) == -1)
		return (NULL);
	if (exp.result == static_buffer)
	{
		if (ft_strcmp(exp.result, token) == 0)
			return (token);
		final_result = ft_strdup(exp.result);
		if (!final_result)
			return (NULL);
		return (final_result);
	}
	final_result = exp.result;
	exp.result = NULL;
	free_expand(&exp);
	return (exp.result);
}

void	expand_tokens(t_master *master)
{
	t_token	*current;
	char	*expanded;

	current = master->token;
	expanded = NULL;
	while (current)
	{
		if (!current->in_single_quotes)
		{
			expanded = expand_var(current->str, master->ctx);
			if (expanded)
			{
				if (expanded != current->str)
				{
					free(current->str);
					current->str = expanded;
				}
			}
			else
				current->str = ft_strdup("");
		}
		current = current->next;
	}
}
