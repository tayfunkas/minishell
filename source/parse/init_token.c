/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:15:34 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 06:33:26 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_whitespace(char *start)
{
	while (ft_isspace(*start))
		start++;
	return (start);
}

static void	handle_unclosed_quotes(void)
{
	write(2, "Unclosed quotes aren't handled\n", 31);
}

static int	process_next_token(t_parser *parser, int *i, 
	t_tok **current, t_tok **head)
{
	parser->start = skip_whitespace(parser->start);
	if (*(parser->start) == '\0')
		return (1);
	*current = process_token(parser, i, *current);
	if (!*current)
	{
		if (parser->unclosed)
		{
			handle_unclosed_quotes();
			return (0);
		}
		free_tokens(*head);
		return (1);
	}
	if (!*head)
		*head = *current;
	return (1);
}

t_tok	*tokenize_inputs(char *input, int max_args)
{
	t_tok		*head;
	t_tok		*current;
	int			i;
	t_parser	parser;

	head = NULL;
	current = NULL;
	i = 0;
	parser.start = input;
	while (*parser.start && i < max_args)
	{
		if (!process_next_token(&parser, &i, &current, &head))
			return (NULL);
	}
	return (head);
}
