/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:34:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 15:22:42 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*char	*handle_quoted_string(char *start, char quote)
{
	char	*end;

	end = ft_strchr(start, quote);
	if (!end)
		end = start + ft_strlen(start);
	return (end);
}*/
