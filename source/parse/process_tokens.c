/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:27:28 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 17:32:34 by kyukang          ###   ########.fr       */
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
		end++;
	while (ft_isspace(*end))
		end++;
	return (end);
}

/*t_token	*add_token_to_list(char *start, int len, int quo_info, t_token *c)
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
}*/

/*t_token *add_token_to_list(char *start, int len, char quote, t_token *c)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    char *cleaned_str = malloc(len + 1);
    if (!cleaned_str)
    {
        free(new_token);
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (start[i] != '"' && start[i] != '\'')
        {
            cleaned_str[j++] = start[i];
        }
    }
    cleaned_str[j] = '\0';

    new_token->str = cleaned_str;
    new_token->flag = quote ? 1 : 0;
    new_token->quote = quote;
    new_token->next = NULL;
    new_token->prev = c;

    if (c)
        c->next = new_token;

    return new_token;
}*/

t_token *add_token_to_list(char *start, int len, char quote, t_token *current)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    char *cleaned_str = malloc(len + 1);
    if (!cleaned_str)
    {
        free(new_token);
        return NULL;
    }

    int j = 0;
    char outer_quote = 0;
    char inner_quote = 0;

    for (int i = 0; i < len; i++)
    {
        if (!outer_quote && (start[i] == '"' || start[i] == '\''))
        {
            outer_quote = start[i];
        }
        else if (outer_quote && start[i] == outer_quote)
        {
            outer_quote = 0;
        }
        else if (outer_quote && !inner_quote && start[i] != outer_quote && (start[i] == '"' || start[i] == '\''))
        {
            inner_quote = start[i];
            cleaned_str[j++] = start[i];
        }
        else if (inner_quote && start[i] == inner_quote)
        {
            inner_quote = 0;
        }
        else
        {
            cleaned_str[j++] = start[i];
        }
    }
    cleaned_str[j] = '\0';

    new_token->str = cleaned_str;
    new_token->flag = quote ? 1 : 0;
    new_token->quote = quote;
    new_token->next = NULL;
    new_token->prev = current;

    if (current)
        current->next = new_token;

    return new_token;
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

/*t_token *process_token(char **start, int *i, t_token *current)
{
    char *end = *start;
    char quote = 0;
    char *token_start = *start;
    int len = 0;

    while (*end)
    {
        if (*end == '"' || *end == '\'')
        {
            char current_quote = *end;
            end++;
            while (*end && *end != current_quote)
                end++;
            if (*end == current_quote)
                end++;
        }
        else if (ft_isspace(*end) || *end == '|')
        {
            break;
        }
        else
        {
            end++;
        }
    }

    len = end - token_start;
    t_token *new_token = add_token_to_list(token_start, len, quote, current);
    if (!new_token)
        return NULL;

    *start = end;
    (*i)++;
    return new_token;
}*/

/*t_token *process_token(char **start, int *i, t_token *current)
{
    char *end = *start;
    char outer_quote = 0;
    char inner_quote = 0;
    char *token_start = *start;
    int len = 0;

    while (*end)
    {
        if (!outer_quote && (*end == '"' || *end == '\''))
        {
            outer_quote = *end;
            end++;
        }
        else if (outer_quote && *end == outer_quote)
        {
            outer_quote = 0;
            end++;
        }
        else if (outer_quote && !inner_quote && *end != outer_quote && (*end == '"' || *end == '\''))
        {
            inner_quote = *end;
            end++;
        }
        else if (inner_quote && *end == inner_quote)
        {
            inner_quote = 0;
            end++;
        }
        else if (!outer_quote && (ft_isspace(*end) || *end == '|'))
        {
            break;
        }
        else
        {
            end++;
        }
    }

    len = end - token_start;
    t_token *new_token = add_token_to_list(token_start, len, outer_quote, current);
    if (!new_token)
        return NULL;

    *start = end;
    (*i)++;
    return new_token;
}*/

t_token *process_token(char **start, int *i, t_token *current)
{
    char *end = *start;
    char outer_quote = 0;
    char inner_quote = 0;
    char *token_start = *start;
    int len = 0;

    // Check for special characters first
    if (*end == '|' || *end == '<' || *end == '>')
    {
        if ((*end == '<' && *(end + 1) == '<') || (*end == '>' && *(end + 1) == '>'))
        {
            len = 2;
        }
        else
        {
            len = 1;
        }
        t_token *new_token = add_token_to_list(token_start, len, 0, current);
        if (!new_token)
            return NULL;
        *start = end + len;
        (*i)++;
        return new_token;
    }

    // Process regular tokens and quoted strings
    while (*end)
    {
        if (!outer_quote && (*end == '"' || *end == '\''))
        {
            outer_quote = *end;
            end++;
        }
        else if (outer_quote && *end == outer_quote)
        {
            outer_quote = 0;
            end++;
        }
        else if (outer_quote && !inner_quote && *end != outer_quote && (*end == '"' || *end == '\''))
        {
            inner_quote = *end;
            end++;
        }
        else if (inner_quote && *end == inner_quote)
        {
            inner_quote = 0;
            end++;
        }
        else if (!outer_quote && (ft_isspace(*end) || *end == '|' || *end == '<' || *end == '>'))
        {
            break;
        }
        else
        {
            end++;
        }
    }

    len = end - token_start;
    t_token *new_token = add_token_to_list(token_start, len, outer_quote, current);
    if (!new_token)
        return NULL;

    *start = end;
    (*i)++;
    return new_token;
}
