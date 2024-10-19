/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:27:28 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/19 19:53:14 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*move_to_next_token(char *end, int in_quo, char quote)
{
	if (*end == quote && in_quo)
		return (end + 1);
	return (end);
}

t_token	*add_token_to_list(char *start, int len, int quo_info, t_token *c)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = (char *)malloc(len + 1);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	ft_strncpy(new_token->str, start, len);
	new_token->str[len] = '\0';
	new_token->flag = quo_info & 0xFF;
	new_token->quote = quo_info >> 8 & 0xFF;
	new_token->next = NULL;
	new_token->prev = c;
	if (c)
		c->next = new_token;
	return (new_token);
}

/*t_token	*process_token(char **start, int *i, t_token *current)
{
	int		quo_info;
	char	*end;
	int		len;
	t_token	*new_token;

	quo_info = 0;
	if (**start == '"' || **start == '\'')
	{
		quo_info = 1 | (**start << 8);
		(*start)++;
		end = get_token_end(*start, &quo_info, (char *)&quo_info + 1);
	}
	else if (**start == '|' && !(quo_info & 0xFF))
		end = *start + 1;
	else
		end = get_token_end(*start, &quo_info, NULL);
	len = end - *start;
	new_token = add_token_to_list(*start, len, quo_info, current);
	if (!new_token)
		return (NULL);
	*start = move_to_next_token(end, quo_info & 0xFF, quo_info >> 8 & 0xFF);
	(*i)++;
	return (new_token);
}*/

t_token *process_token(char **start, int *i, t_token *current)
{
    char *token_start = *start;
    char *token_end = *start;
    int in_quotes = 0;
    char quote_char = 0;
    char *result = NULL;
    int result_len = 0;

    while (**start)
    {
        if ((**start == '"' || **start == '\'') && !in_quotes)
        {
            in_quotes = 1;
            quote_char = **start;
            (*start)++;
        }
        else if (**start == quote_char && in_quotes)
        {
            in_quotes = 0;
            quote_char = 0;
            (*start)++;
        }
        else if (**start == '|' && !in_quotes)
        {
            if (token_start == *start)
            {
                token_end = *start + 1;
                (*start)++;
            }
            break;
        }
        else
        {
            if (!result)
                result = malloc(strlen(*start) + 1);
            result[result_len++] = **start;
            (*start)++;
        }

        if (!in_quotes && (ft_isspace(**start) || **start == '\0'))
            break;
    }

    if (result)
    {
        result[result_len] = '\0';
        t_token *new_token = add_token_to_list(result, result_len, 0, current);
        free(result);
        if (!new_token)
            return NULL;
        (*i)++;
        return new_token;
    }
    else if (token_end > token_start)
    {
        int len = token_end - token_start;
        t_token *new_token = add_token_to_list(token_start, len, 0, current);
        if (!new_token)
            return NULL;
        (*i)++;
        return new_token;
    }

    return NULL;
}
