/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:39:00 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:25:18 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	type_tokens(t_tok *token)
{
	if (token->prev && (token->prev->type == TRUNC
			|| token->prev->type == APPEND || token->prev->type == INPUT))
		token->type = FILENAME;
	else if (ft_strcmp(token->str, "") == 0)
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
	else if (token->prev == NULL || token->prev->type == PIPE
		|| token->prev->type == END)
		token->type = CMD;
	else
		token->type = ARG;
}

void	assign_token_types(t_tok *tokens)
{
	t_tok	*current;

	current = tokens;
	while (current)
	{
		type_tokens(current);
		current = current->next;
	}
}
