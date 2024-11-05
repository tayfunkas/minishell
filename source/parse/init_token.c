/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:15:34 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 03:18:43 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_whitespace(char *start)
{
	while (ft_isspace(*start))
		start++;
	return (start);
}

static int	process_next_token(t_parser *pars, int *i, t_tok **current,
	t_tok **head)
{
	pars->start = skip_whitespace(pars->start);
	if (*pars->start == '\0')
		return (0);
	if (!process_token(pars, i, current, head))
	{
		if (pars->unclosed)
			write(2, "Unclosed quotes aren't handled\n", 31);
		free_tokens(*head);
		*head = NULL;
		*current = NULL;
		return (0);
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
	t_parser	pars;

	head = NULL;
	current = NULL;
	i = 0;
	pars.start = input;
	while (*pars.start && i < max_args)
	{
		if (!process_next_token(&pars, &i, &current, &head))
		{
			free_tokens(head);
			return (NULL);
		}
		if (!head)
			head = current;
	}
	return (head);
}
