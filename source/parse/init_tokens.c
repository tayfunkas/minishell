/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 21:10:03 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/22 13:56:19 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_whitespace(char *start)
{
	while (ft_isspace(*start))
		start++;
	return (start);
}

t_token	*tokenize_inputs(char *input, int max_args)
{
	t_token	*head;
	t_token	*current;
	int		i;
	char	*start;

	head = NULL;
	current = NULL;
	i = 0;
	start = input;
	while (*start && i < max_args)
	{
		start = skip_whitespace(start);
		if (*start == '\0')
			break ;
		current = process_token(&start, &i, current);
		if (!current)
		{
			free_tokens(head);
			return (NULL);
		}
		if (!head)
			head = current;
	}
	return (head);
}
