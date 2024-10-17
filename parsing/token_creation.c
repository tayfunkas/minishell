/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 21:10:03 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 14:01:22 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_new_tok(char *start, int len, int in_quo, t_token *current)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->str = (char *)malloc(len + 1);
	ft_strncpy(new_token->str, start, len);
	new_token->str[len] = '\0';
	new_token->flag = in_quo;
	new_token->next = NULL;
	new_token->prev = current;
	if (current)
		current->next = new_token;
	return (new_token);
}

char	*get_token_end(char *start, int *in_quo, char *quote)
{
	char	*end;

	if (*in_quo && quote)
	{
		end = ft_strchr(start, *quote);
		if (!end)
			end = start + ft_strlen(start);
	}
	else
	{
		end = start;
		while (*end && !ft_isspace(*end) && *end != '|')
			end++;
	}
	return (end);
}

char	*handle_regular_token(char *start)
{
	char	*end;

	end = start;
	while (*end && !ft_isspace(*end) && *end != '|')
		end++;
	return (end);
}

char	*move_to_next_token(char *end, int in_quo, char quote)
{
	if (*end == quote && in_quo)
		return (end + 1);
	return (end);
}

t_token	*tokenize_input(char *input, int max_args)
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
		if (!head)
			head = current;
	}
	return (head);
}
