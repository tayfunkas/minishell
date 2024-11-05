/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:37:51 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 20:47:16 by kyukang          ###   ########.fr       */
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

static int	process_token_loop(char *token, t_expand *exp, t_ctx *ctx)
{
	static char	static_buffer[1024];

	while (token[exp->tok_idx])
	{
		if (handle_quotes(token, exp))
			continue ;
		if (token[exp->tok_idx] == '$' && !exp->in_single_quote)
		{
			if (handle_env_var_exp(token, exp, ctx) == -1)
				return (-1);
		}
		else
			exp->result[exp->result_idx++] = token[exp->tok_idx++];
		if (exp->result_idx >= (int)exp->result_size - 1)
		{
			if (expand_result_buffer(exp, static_buffer) == -1)
				return (-1);
		}
	}
	exp->result[exp->result_idx] = '\0';
	return (0);
}

char	*expand_var(char *token, t_ctx *ctx)
{
	static char	static_buffer[1024];
	t_expand	exp;
	char		*final_result;

	exp.result = static_buffer;
	exp.result_size = 1024;
	exp.in_single_quote = 0;
	exp.in_double_quote = 0;
	exp.tok_idx = 0;
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
	return (final_result);
}

void	expand_tokens(t_tok *head, t_ctx *ctx)
{
	t_tok	*current;
	char	*expanded;

	current = head;
	while (current)
	{
		if (!current->in_single_quotes)
		{
			expanded = expand_var(current->str, ctx);
			if (expanded)
			{
				if (expanded != current->str)
				{
					free(current->str);
					current->str = expanded;
				}
			}
			else
			{
				free(current->str);
				current->str = ft_strdup("");
				if (!current->str)
				{
					perror("strdup");
					return ;
				}
			}
		}
		current = current->next;
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
