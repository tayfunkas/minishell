/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:41:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 17:56:50 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_syntax(t_tok *cur)
{
	if (cur->type == APPEND && cur->next && cur->next->type == TRUNC)
		return (0);
	if ((cur->type == TRUNC || cur->type == APPEND || cur->type == INPUT)
		&& (!cur->next || cur->next->type != FILENAME))
		return (0);
	if (cur->type == HEREDOC && !cur->next)
		return (0);
	if ((cur->type == TRUNC || cur->type == APPEND
			|| cur->type == INPUT || cur->type == HEREDOC)
		&& cur->next
		&& (cur->next->type == TRUNC || cur->next->type == APPEND
			|| cur->next->type == INPUT || cur->next->type == HEREDOC))
		return (0);
	return (1);
}

int	check_pipe_syntax(t_tok *current, int *valid_before_pipe, int *pipe_allowed)
{
	if (current->type == PIPE)
	{
		if (!*valid_before_pipe || !current->next
			|| (current->next && current->next->type == PIPE))
			return (0);
		*valid_before_pipe = 0;
		*pipe_allowed = 0;
	}
	else if (current->type == CMD || current->type == ARG
		|| current->type == FILENAME)
	{
		*valid_before_pipe = 1;
		*pipe_allowed = 1;
	}
	return (1);
}

int	check_syntax(t_tok *tokens)
{
	t_tok	*current;
	int		valid_before_pipe;
	int		pipe_allowed;

	current = tokens;
	valid_before_pipe = 0;
	pipe_allowed = 0;
	while (current)
	{
		if (!check_redirection_syntax(current)
			|| !check_pipe_syntax(current, &valid_before_pipe, &pipe_allowed))
			return (0);
		current = current->next;
	}
	if (current && current->type == PIPE)
		return (0);
	return (1);
}
