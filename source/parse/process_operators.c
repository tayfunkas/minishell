/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:20:52 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 21:16:40 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*process_arg(t_parser *pars, t_tok *new_token)
{
	t_tok	*arg_token;

	pars->start = pars->end;
	while (*pars->end && !ft_isspace(*pars->end) && *pars->end != '|'
		&& *pars->end != '<' && *pars->end != '>')
		pars->end++;
	pars->len = pars->end - pars->start;
	arg_token = tok_to_list(pars->start, pars->len, 0, new_token);
	if (!arg_token)
		return (NULL);
	pars->start = pars->end;
	return (arg_token);
}

t_tok	*process_operators(t_parser *pars, int *i, t_tok *current)
{
	t_tok	*new_token;
	t_tok	*arg_token;

	if ((*pars->end == '<' && *(pars->end + 1) == '<')
		|| (*pars->end == '>' && *(pars->end + 1) == '>'))
		pars->len = 2;
	else
		pars->len = 1;
	new_token = tok_to_list(pars->start, pars->len, 0, current);
	if (!new_token)
		return (NULL);
	pars->start = pars->end + pars->len;
	(*i)++;
	while (*pars->end && ft_isspace(*pars->end))
		pars->end++;
	if (*pars->end && !ft_isspace(*pars->end) && *pars->end != '|'
		&& *pars->end != '<' && *pars->end != '>')
	{
		arg_token = process_arg(pars, new_token);
		if (arg_token)
			return (NULL);
		(*i)++;
	}
	return (new_token);
}

t_tok	*process_quoted_operators(t_parser *pars, int *i, t_tok *cur)
{
	t_tok	*new_token;
	char	initial_quote;

	initial_quote = *pars->end;
	if ((*pars->end == '"' || *pars->end == '\'')
		&& (*(pars->end + 1) == '|' || *(pars->end + 1) == '<'
			|| *(pars->end + 1) == '>'))
	{
		pars->len = 2;
		if ((*(pars->end + 1) == '<' && *(pars->end + 2) == '<')
			|| (*(pars->end + 1) == '>' && *(pars->end + 2) == '>'))
			pars->len = 3;
		if (*(pars->end + pars->len) == initial_quote)
		{
			pars->len++;
			new_token = tok_to_list(pars->start, pars->len, initial_quote, cur);
			if (!new_token)
				return (NULL);
			pars->start = pars->end + pars->len;
			pars->end = pars->start;
			(*i)++;
			return (new_token);
		}
	}
	return (NULL);
}

t_tok	*process_initial_operators(t_parser *pars, int *i, t_tok *current)
{
	t_tok	*new_token;

	new_token = process_quoted_operators(pars, i, current);
	if (new_token != NULL)
		return (new_token);
	if (*pars->end == '|' || *pars->end == '<' || *pars->end == '>')
		return (process_operators(pars, i, current));
	return (NULL);
}
