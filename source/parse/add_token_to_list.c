/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_to_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:41:33 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/22 16:42:29 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*init_new_token(char *cleaned_str, char quote, t_token *current)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
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

//s stands for start
static void	process_quote(char *s, int len, char *cleaned_str, int i)
{
	int		j;
	char	out;
	char	in;

	j = 0;
	out = 0;
	in = 0;
	while (i < len)
	{
		if (!out && (s[i] == '"' || s[i] == '\''))
			out = s[i];
		else if (out && s[i] == out)
			out = 0;
		else if (out && !in && s[i] != out && (s[i] == '"' || s[i] == '\''))
		{
			in = s[i];
			cleaned_str[j++] = s[i];
		}
		else if (in && s[i] == in)
			in = 0;
		else
			cleaned_str[j++] = s[i];
		i++;
	}
	cleaned_str[j] = '\0';
}

static char	*clean_token_str(char *start, int len)
{
	char	*cleaned_str;
	int		i;

	i = 0;
	cleaned_str = malloc(len + 1);
	if (!cleaned_str)
		return (NULL);
	process_quote(start, len, cleaned_str, i);
	return (cleaned_str);
}

t_token	*add_token_to_list(char *start, int len, char quote, t_token *current)
{
	t_token	*new_token;
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