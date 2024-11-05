/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_to_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:34:54 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 20:56:34 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tok	*init_new_token(char *cleaned_str, char quote, t_tok *current)
{
	t_tok	*new_token;

	new_token = (t_tok *)malloc(sizeof(t_tok));
	if (!new_token)
		return (NULL);
	new_token->str = cleaned_str;
	if (quote)
		new_token->flag = 1;
	else
		new_token->flag = 0;
	new_token->quote = quote;
	new_token->in_single_quotes = 0;
	new_token->next = NULL;
	new_token->prev = current;
	return (new_token);
}

static void	go_through_quotes(char *start, t_quote *quote, char *cleaned_str)
{
	if (!quote->outer && (start[quote->i] == '"' || start[quote->i] == '\''))
		quote->outer = start[quote->i++];
	else if (quote->outer && start[quote->i] == quote->outer)
	{
		quote->outer = 0;
		quote->i++;
	}
	else if (quote->outer && !quote->inner && start[quote->i] != quote->outer
		&& (start[quote->i] == '"' || start[quote->i] == '\''))
	{
		quote->inner = start[quote->i];
		cleaned_str[quote->j++] = start[quote->i++];
	}
	else if (quote->inner && start[quote->i] == quote->inner)
	{
		quote->inner = 0;
		cleaned_str[quote->j++] = start[quote->i++];
	}
	else
		cleaned_str[quote->j++] = start[quote->i++];
}

static char	*clean_token_str(char *start, int len)
{
	char	*cleaned_str;
	t_quote	quote;

	cleaned_str = malloc(len + 1);
	if (!cleaned_str)
	{
		write(2, "Memory allocation failed\n", 25);
		return (NULL);
	}
	quote.outer = 0;
	quote.inner = 0;
	quote.i = 0;
	quote.j = 0;
	while (quote.i < len)
	{
		go_through_quotes(start, &quote, cleaned_str);
	}
	cleaned_str[quote.j] = '\0';
	return (cleaned_str);
}

t_tok	*tok_to_list(char *start, int len, char quote, t_tok *current)
{
	t_tok	*new_token;
	char	*cleaned_str;

	cleaned_str = clean_token_str(start, len);
	if (!cleaned_str)
		return (NULL);
	new_token = init_new_token(cleaned_str, quote, current);
	if (!new_token)
	{
		free(cleaned_str);
		return (NULL);
	}
	if (current)
		current->next = new_token;
	return (new_token);
}
