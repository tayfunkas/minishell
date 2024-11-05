/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:18:57 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 04:02:40 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_parsers(t_parser *parser)
{
	parser->end = parser->start;
	parser->outer = 0;
	parser->inner = 0;
	parser->len = 0;
}

static void	advance_to_token_end(t_parser *pars)
{
	while (*pars->end)
	{
		if (!pars->outer && (*pars->end == '"' || *pars->end == '\''))
		{
			pars->outer = *pars->end;
			pars->end++;
		}
		else if (pars->outer && *pars->end == pars->outer)
		{
			pars->outer = 0;
			pars->end++;
		}
		else
			pars->end++;
		if (!pars->outer && (ft_isspace(*pars->end) || *pars->end == '|'
				|| *pars->end == '<' || *pars->end == '>'))
			return ;
	}
}

int	process_initial_token(t_parser *pars, int *i, t_tok **current, t_tok **head)
{
	t_tok	*new_token;

	new_token = process_initial_operators(pars, i, *current);
	if (new_token != NULL)
	{
		*current = new_token;
		if (!*head)
			*head = new_token;
		return (1);
	}
	return (0);
}

int	process_token(t_parser *pars, int *i, t_tok **current, t_tok **head)
{
	t_tok	*new_token;
	int		entirely_in_single_quotes;

	init_parsers(pars);
	if (process_initial_token(pars, i, current, head))
		return (1);
	entirely_in_single_quotes = (*pars->end == '\'');
	advance_to_token_end(pars);
	pars->len = pars->end - pars->start;
	if (pars->outer)
	{
		pars->unclosed = pars->outer;
		return (0);
	}
	new_token = tok_to_list(pars->start, pars->len, pars->outer, *current);
	if (!new_token)
		return (0);
	new_token->in_single_quotes = entirely_in_single_quotes;
	*current = new_token;
	if (!*head)
		*head = new_token;
	pars->start = pars->end;
	(*i)++;
	return (1);
}
