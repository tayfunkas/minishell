/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:31:05 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:18 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prep_args(t_tok *tokens, int token_count, char **args, t_ctx *ctx)
{
	t_tok	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < token_count)
	{
		if (current->type == CMD || current->type == ARG)
		{
			if (current->in_single_quotes)
				args[i] = strdup(current->str);
			else
				args[i] = expand_var(current->str, ctx);
			i++;
		}
		else if (current->type == TRUNC || current->type == APPEND
			|| current->type == INPUT || current->type == HEREDOC)
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		else if (current->type == PIPE)
			break ;
		if (current)
			current = current->next;
	}
	args[i] = NULL;
}

char	**allocate_args(int token_count)
{
	char	**args;

	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (args == NULL)
		perror("malloc failed");
	return (args);
}

int	count_args(t_tok *tokens)
{
	int		count;
	t_tok	*current;

	count = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == CMD || current->type == ARG)
			count++;
		else if (current->type == TRUNC || current->type == APPEND
			|| current->type == INPUT || current->type == HEREDOC)
		{
			current = current->next;
			if (!current)
				break ;
		}
		current = current->next;
	}
	return (count);
}
