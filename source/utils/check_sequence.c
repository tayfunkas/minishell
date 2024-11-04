/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:50:27 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 17:50:53 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_quote_state(char current_char, char *quote_char)
{
	if (current_char == '\'' || current_char == '"')
	{
		if (*quote_char == 0)
			*quote_char = current_char;
		else if (*quote_char == current_char)
			*quote_char = 0;
		return (1);
	}
	return (0);
}

static int	check_redir_sequence(const char **ptr)
{
	char	c;
	int		count;

	c = **ptr;
	count = 0;
	while (**ptr == c)
	{
		count++;
		(*ptr)++;
	}
	if (count > 2)
		return (0);
	while (**ptr && ft_isspace(**ptr))
		(*ptr)++;
	if (**ptr == '\0' || **ptr == '>' || **ptr == '<' || **ptr == '|')
		return (0);
	while (**ptr
		&& !ft_isspace(**ptr) && **ptr != '>' && **ptr != '<' && **ptr != '|')
		(*ptr)++;
	return (1);
}

static int	check_pipe_sequence(const char **ptr)
{
	(*ptr)++;
	while (**ptr && ft_isspace(**ptr))
		(*ptr)++;
	return (1);
}

static int	ptr_is_redir_or_pipe(const char *ptr)
{
	if (*ptr == '>' || *ptr == '<')
	{
		if (!check_redir_sequence(&ptr))
			return (0);
	}
	else if (*ptr == '|')
	{
		if (!check_pipe_sequence(&ptr))
			return (0);
	}
	return (1);
}

int	check_invalid_sequences(const char *input)
{
	const char	*ptr;
	char		quote_char;

	ptr = input;
	quote_char = 0;
	while (*ptr)
	{
		if (update_quote_state(*ptr, &quote_char))
		{
			ptr++;
			continue ;
		}
		if (quote_char != 0)
		{
			ptr++;
			continue ;
		}
		if (!ptr_is_redir_or_pipe(ptr))
			return (0);
		else
			ptr++;
	}
	return (1);
}
