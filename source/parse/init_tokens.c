/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 21:10:03 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 18:24:16 by kyukang          ###   ########.fr       */
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

/*char	*handle_regular_token(char *start)
{
	char	*end;

	end = start;
	while (*end && !ft_isspace(*end) && *end != '|')
		end++;
	return (end);
}*/

/*t_token	*create_new_tok(char *start, int len, int in_quo, t_token *current)
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
}*/

/*t_token	*add_token_to_list(char *start, char *end,
int in_quo, char quote, t_token *c)
{
	t_token	*new_token;
	int		len;

	len = end - start;
	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->str = (char *)malloc(len + 1);
	ft_strncpy(new_token->str, start, len);
	new_token->str[len] = '\0';
	new_token->flag = in_quo;
	new_token->quote = quote;
	new_token->next = NULL;
	new_token->prev = c;
	if (c)
		c->next = new_token;
	return (new_token);
}

t_token	*process_token(char **start, int *i, t_token *current)
{
	int		in_quo;
	char	quote;
	char	*end;

	in_quo = 0;
	quote = '\0';
	if (**start == '"' || **start == '\'')
	{
		in_quo = 1;
		quote = **start;
		(*start)++;
		end = get_token_end(*start, &in_quo, &quote);
	}
	else if (**start == '|' && !in_quo)
		end = *start + 1;
	else
		end = get_token_end(*start, &in_quo, NULL);
	current = add_token_to_list(*start, end, in_quo, quote, current);
	if (in_quo)
		*start = move_to_next_token(end, in_quo, quote);
	else
		*start = move_to_next_token(end, in_quo, '\0');
	(*i)++;
	return (current);
}*/
