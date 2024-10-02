/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:34:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/02 14:41:50 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize_input(char *input, int max_args)
{
	t_token	*head;
	t_token	*current;
	int		i;
	int		inside_quotes;
	char	*start;
	char	*end;
	char	quote;
	t_token	*new_token;
	int		len;

	head = NULL;
	current = NULL;
	start = input;
	i = 0;
	inside_quotes = 0;
	while (*start && i < max_args)
	{
		while (ft_isspace(*start))
			start++;
		if (*start == '"' || *start == '\'')
		{
			inside_quotes = 1;
			quote = *start;
			start++;
			end = ft_strchr(start, quote);
		}
		else
		{
			inside_quotes = 0;
			end = start;
			while (*end && !ft_isspace(*end))
				end++;
		}
		if (end)
		{
			new_token = (t_token *)malloc(sizeof(t_token));
			len = end - start;
			new_token->str = (char *)malloc(len + 1);
			ft_strncpy(new_token->str, start, len);
			new_token->str[len] = '\0';
			new_token->flag = inside_quotes;
			new_token->next = NULL;
			new_token->prev = current;
			if (current)
				current->next = new_token;
			else
				head = new_token;
			current = new_token;
			i++;
			if (*end)
				start = end + 1;
			else
				start = end;
		}
	}
	return (head);
}

void	assign_token_types(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		type_tokens(current);
		current = current->next;
	}
}

void	type_tokens(t_token *token)
{
	if (ft_strcmp(token->str, "") == 0)
		token->type = EMPTY;
	else if (ft_strcmp(token->str, ">") == 0 && token->flag == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->str, ">>") == 0 && token->flag == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->str, "<") == 0 && token->flag == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->str, "|") == 0 && token->flag == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0 && token->flag == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}
