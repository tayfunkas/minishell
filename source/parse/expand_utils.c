/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:56:26 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 18:12:42 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(char *token, t_expand *exp)
{
	if (token[exp->tok_idx] == '\'' && !exp->in_double_quote)
	{
		exp->in_single_quote = !exp->in_single_quote;
		exp->result[exp->result_idx++] = token[exp->tok_idx++];
	}
	else if (token[exp->tok_idx] == '"' && !exp->in_single_quote)
	{
		exp->in_double_quote = !exp->in_double_quote;
		exp->result[exp->result_idx++] = token[exp->tok_idx++];
	}
	if (exp->in_single_quote)
	{
		exp->result[exp->result_idx++] = token[exp->tok_idx++];
		return (1);
	}
	return (0);
}

static int	expand_result_buffer_if_needed(t_expand *exp, char *static_buffer)
{
	if (exp->result == static_buffer)
	{
		exp->result = malloc(exp->result_size * 2);
		if (!exp->result)
		{
			free(exp->env_var);
			return (-1);
		}
		ft_memcpy(exp->result, static_buffer, exp->result_idx);
	}
	else
	{
		exp->new_result = malloc(exp->result_size * 2);
		if (!exp->new_result)
		{
			free(exp->env_var);
			free(exp->result);
			return (-1);
		}
		ft_memcpy(exp->new_result, exp->result, exp->result_idx);
		free(exp->result);
		exp->result = exp->new_result;
	}
	exp->result_size *= 2;
	return (0);
}

static int	process_env_var(t_expand *exp, t_ctx *ctx,
	char *static_buffer)
{
	size_t	env_value_len;

	if (ft_strncmp(ctx->our_env[exp->env_index], exp->env_var,
			ft_strlen(exp->env_var)) == 0
		&& ctx->our_env[exp->env_index][ft_strlen(exp->env_var)] == '=')
	{
		exp->env_value = ctx->our_env[exp->env_index]
			+ ft_strlen(exp->env_var) + 1;
		env_value_len = ft_strlen(exp->env_value);
		if (exp->result_idx + env_value_len >= exp->result_size)
		{
			if (expand_result_buffer_if_needed(exp, static_buffer) == -1)
				return (-1);
		}
		ft_strcpy(exp->result + exp->result_idx, exp->env_value);
		exp->result_idx += env_value_len;
		return (1);
	}
	return (0);
}

static int	expand_env_var(t_expand *exp, t_ctx *ctx,
	char *static_buffer)
{
	exp->env_index = 0;
	while (ctx->our_env[exp->env_index])
	{
		if (process_env_var(exp, ctx, static_buffer))
			break ;
		exp->env_index++;
	}
	free(exp->env_var);
	return (0);
}

int	handle_env_var_exp(char *tok, t_expand *exp, t_ctx *ctx)
{
	int			i;
	static char	static_buffer[1024];

	if (!tok[exp->tok_idx + 1] || tok[exp->tok_idx + 1] == ' '
		|| tok[exp->tok_idx + 1] == '"' || tok[exp->tok_idx + 1] == '\'')
		exp->result[exp->result_idx++] = tok[exp->tok_idx++];
	else
	{
		exp->tok_idx++;
		i = exp->tok_idx;
		while (tok[i] && (ft_isalnum(tok[i]) || tok[i] == '_' || tok[i] == '?'))
			i++;
		exp->env_var = ft_strndup(tok + exp->tok_idx, i - exp->tok_idx);
		if (exp->env_var)
		{
			if (expand_env_var(exp, ctx, static_buffer) == -1)
			{
				free(exp->env_var);
				return (-1);
			}
			exp->env_var = NULL;
		}
		exp->tok_idx = i;
	}
	return (0);
}
