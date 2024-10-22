/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:02:22 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/22 18:22:51 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prep_args(t_token *tokens, int token_count, char **args,
			t_exec_context *ctx)
{
	t_token	*current;
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
			current = current->next;
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
