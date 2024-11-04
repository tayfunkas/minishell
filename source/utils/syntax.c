/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:41:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 16:51:22 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_invalid_sequences(const char *input)
{
	const char *ptr = input;
	char quote_char = 0;

	while (*ptr)
	{
		if (*ptr == '\'' || *ptr == '"')
		{
			quote_char = (quote_char == 0) ? *ptr : (quote_char == *ptr ? 0 : quote_char);
			ptr++;
			continue;
		}
		if (quote_char != 0)
		{
			ptr++;
			continue;
		}
		if (*ptr == '>' || *ptr == '<')
		{
			char c = *ptr;
			int count = 0;
			while (*ptr == c)
			{
				count++;
				ptr++;
			}
			if (count > 2)
				return 0;  // Invalid: more than two consecutive redirection operators
			// Skip whitespace
			while (*ptr && ft_isspace(*ptr))
				ptr++;
			// Check if there's a filename or another token
			if (*ptr == '\0' || *ptr == '>' || *ptr == '<' || *ptr == '|')
				return 0;  // Invalid: redirection not followed by a filename
			// Skip the filename or token
			while (*ptr && !ft_isspace(*ptr) && *ptr != '>' && *ptr != '<' && *ptr != '|')
				ptr++;
		}
		else if (*ptr == '|')
		{
			ptr++;
			// Skip whitespace after pipe
			while (*ptr && ft_isspace(*ptr))
				ptr++;
			// Check if pipe is at the end
		   /* if (*ptr == '\0')
				return 0;  */
		}
		else
			ptr++;
	}
	return 1;
}

int check_syntax(t_tok *tokens)
{
	t_tok *current = tokens;
	int valid_before_pipe = 0;
	int pipe_allowed = 0;

	while (current)
	{
		// Check for >>> error
		if (current->type == APPEND && current->next && current->next->type == TRUNC)
			return 0;

		// Check if a redirection (except HEREDOC) is followed by a filename
		if ((current->type == TRUNC || current->type == APPEND || current->type == INPUT)
			&& (!current->next || current->next->type != FILENAME))
			return 0;

		// Check if HEREDOC is followed by a delimiter
		if (current->type == HEREDOC && !current->next)
			return 0;

		// Check if two redirections follow each other
		if ((current->type == TRUNC || current->type == APPEND || current->type == INPUT || current->type == HEREDOC)
			&& current->next && (current->next->type == TRUNC || current->next->type == APPEND || current->next->type == INPUT || current->next->type == HEREDOC))
			return 0;

		// Check pipe rules
		if (current->type == PIPE)
		{
			if (!valid_before_pipe || !current->next)
				return 0;
			valid_before_pipe = 0;
			pipe_allowed = 0;
		}
		else if (current->type == CMD || current->type == ARG || current->type == FILENAME)
		{
			valid_before_pipe = 1;
			pipe_allowed = 1;
		}

		// Check for empty commands between pipes
		if (current->type == PIPE && current->next && current->next->type == PIPE)
			return 0;

		current = current->next;
	}

	// Check if the last token is a pipe
	if (current && current->type == PIPE)
		return 0;

	return 1;
}