/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:32:37 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/03 20:10:45 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_whitespace(char *start)
{
	while (ft_isspace(*start))
		start++;
	return (start);
}

static t_token	*handle_special_characters(t_parser *parser, int *i,
	t_token *current)
{
	t_token	*new_token;
	t_token	*arg_token;

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
	while (*parser->end && ft_isspace(*parser->end))
		parser->end++;
	if (*parser->end && !ft_isspace(*parser->end) && *parser->end != '|'
		&& *parser->end != '<' && *parser->end != '>')
	{
		parser->start = parser->end;
		while (*parser->end && !ft_isspace(*parser->end) && *parser->end != '|'
			&& *parser->end != '<' && *parser->end != '>')
			parser->end++;
		parser->len = parser->end - parser->start;
		arg_token = add_token_to_list(parser->start, parser->len, 0, new_token);
		if (!arg_token)
			return (NULL);
		parser->start = parser->end;
		(*i)++;
	}
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
	char	initial_quote;

	entirely_in_single_quotes = 0;
	init_parsers(parser);
	initial_quote = *parser->end;
	if ((*parser->end == '"' || *parser->end == '\'')
		&& (*(parser->end + 1) == '|' || *(parser->end + 1) == '<'
			|| *(parser->end + 1) == '>'))
	{
		parser->len = 2;
		if ((*(parser->end + 1) == '<' && *(parser->end + 2) == '<')
			|| (*(parser->end + 1) == '>' && *(parser->end + 2) == '>'))
		{
			parser->len = 3;
		}
		if (*(parser->end + parser->len) == initial_quote)
		{
			parser->len++;
			new_token = add_token_to_list(parser->start, parser->len, initial_quote, current);
			if (!new_token)
				return (NULL);
			parser->start = parser->end + parser->len;
			parser->end = parser->start;
			(*i)++;
			return (new_token);
		}
	}
	else if (*parser->end == '|' || *parser->end == '<' || *parser->end == '>')
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
	if (parser->outer_quote)
	{
		parser->unclosed_quote = parser->outer_quote;
		return (NULL);
	}
	new_token = add_token_to_list(parser->start, parser->len,
			parser->outer_quote, current);
	if (!new_token)
		return (NULL);
	new_token->in_single_quotes = entirely_in_single_quotes;
	parser->start = parser->end;
	(*i)++;
	return (new_token);
}

t_token	*tokenize_inputs(char *input, int max_args)
{
	t_token		*head;
	t_token		*current;
	int			i;
	t_parser	parser;

	head = NULL;
	current = NULL;
	i = 0;
	parser.start = input;
	while (*parser.start && i < max_args)
	{
		parser.start = skip_whitespace(parser.start);
		if (*parser.start == '\0')
			break ;
		current = process_token(&parser, &i, current);
		if (!current)
		{
			if (parser.unclosed_quote)
			{
				write(2, "Unclosed quotes aren't handled\n", 31);
				return (NULL);
			}
			free_tokens(head);
			return (NULL);
		}
		if (!head)
			head = current;
	}
	return (head);
}
