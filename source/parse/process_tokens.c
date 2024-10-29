/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:27:28 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/29 21:00:16 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_special_characters(t_parser *parser, int *i,
	t_token *current)
{
	t_token	*new_token;

	if ((*parser->end == '<' && *(parser->end + 1) == '<')
		|| (*parser->end == '>' && *(parser->end + 1) == '>'))
		parser->len = 2;
	else
		parser->len = 1;
	new_token = add_token_to_list(parser->start, parser->len, 0, current);
	if (!new_token)
		return (NULL);
	parser->start = parser->end + parser->len;
	(*i)++;
	return (new_token);
}

static void	process_quotes(t_parser *parser)
{
	if (!parser->outer_quote && (*parser->end == '"' || *parser->end == '\''))
	{
		parser->outer_quote = *parser->end;
		parser->end++;
	}
	else if (parser->outer_quote && *parser->end == parser->outer_quote)
	{
		parser->outer_quote = 0;
		parser->end++;
	}
	else
		parser->end++;
}

static void	init_parsers(t_parser *parser)
{
	parser->end = parser->start;
	parser->outer_quote = 0;
	parser->inner_quote = 0;
	parser->len = 0;
}

t_token	*process_token(t_parser *parser, int *i, t_token *current)
{
	t_token	*new_token;
	int		entirely_in_single_quotes;

	entirely_in_single_quotes = 0;
	init_parsers(parser);
	if (*parser->end == '|' || *parser->end == '<' || *parser->end == '>')
		return (handle_special_characters(parser, i, current));
	entirely_in_single_quotes = (*parser->end == '\'');
	while (*parser->end)
	{
		process_quotes(parser);
		if (!parser->outer_quote && (ft_isspace(*parser->end)
				|| *parser->end == '|' || *parser->end == '<'
				|| *parser->end == '>'))
			break ;
	}
	parser->len = parser->end - parser->start;
	new_token = add_token_to_list(parser->start, parser->len,
			parser->outer_quote, current);
	if (!new_token)
		return (NULL);
	new_token->in_single_quotes = entirely_in_single_quotes;
	parser->start = parser->end;
	(*i)++;
	return (new_token);
}
