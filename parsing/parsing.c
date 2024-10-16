/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:34:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/16 14:51:23 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*t_token *tokenize_input(char *input, int max_args)
{
	t_token	*head = NULL;
	t_token	*current = NULL;
	int	i = 0;
	int	in_quo = 0;
	char	*start = input;
	char	*end;
	char	quote;

	while (*start && i < max_args)
	{
		// Skip leading whitespace
		while (ft_isspace(*start))
			start++;
		if (*start == '\0')
			break;
		if (*start == '"' || *start == '\'')
        	{
            // Handle quoted strings
			in_quo = 1;
			quote = *start;
			start++;
			end = ft_strchr(start, quote);
			if (!end)
				end = start + ft_strlen(start);
		}
		else if (*start == '|' && !in_quo)
		{
		// Handle pipe as a separate token
			end = start + 1;
		}
		else
		{
		// Handle regular tokens
			in_quo = 0;
			end = start;
			while (*end && !ft_isspace(*end) && *end != '|')
				end++;
		}
		// Create and add the new token
		int len = end - start;
		t_token *new_token = (t_token *)malloc(sizeof(t_token));
		new_token->str = (char *)malloc(len + 1);
		ft_strncpy(new_token->str, start, len);
		new_token->str[len] = '\0';
		new_token->flag = in_quo;
		new_token->next = NULL;
		new_token->prev = current;
		if (current)
			current->next = new_token;
		else
			head = new_token;
		current = new_token;
		i++;
		// Move to next token
		if (*end == quote && in_quo)
			start = end + 1;
		else
			start = end;
		in_quo = 0;
	}
	return head;
}*/

char	*handle_quoted_string(char *start, char quote)
{
	char	*end;

	end = ft_strchr(start, quote);
	if (!end)
		end = start + ft_strlen(start);
	return (end);
}

t_token	*process_token(char **start, int *i, t_token *current)
{
	int		in_quo;
	char	quote;
	char	*end;

	in_quo = 0;
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
	current = add_token_to_list(*start, end, in_quo, current);
	if (in_quo)
		*start = move_to_next_token(end, in_quo, quote);
	else
		*start = move_to_next_token(end, in_quo, '\0');
	(*i)++;
	return (current);
}

t_token	*add_token_to_list(char *start, char *end, int in_quo, t_token *c)
{
	t_token	*new_token;
	int		len;

	len = end - start;
	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->str = (char *)malloc(len + 1);
	ft_strncpy(new_token->str, start, len);
	new_token->str[len] = '\0';
	new_token->flag = in_quo;
	new_token->next = NULL;
	new_token->prev = c;
	if (c)
		c->next = new_token;
	return (new_token);
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
	else if (ft_strcmp(token->str, "<<") == 0 && token->flag == 0)
		token->type = HEREDOC;
	else if (ft_strcmp(token->str, "|") == 0 && token->flag == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0 && token->flag == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}
