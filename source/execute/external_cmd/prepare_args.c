/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:31:05 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/03 18:27:06 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	prep_args(t_token *tokens, int token_count, char **args,
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
}*/

void prep_args(t_token *tokens, int token_count, char **args, t_exec_context *ctx)
{
    t_token *current;
    int i;

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
        else if (current->type == TRUNC || current->type == APPEND ||
                 current->type == INPUT || current->type == HEREDOC)
        {
            // Skip the redirection operator
            current = current->next;
            if (current) // Skip the filename
                current = current->next;
            continue; // Continue to the next iteration without incrementing current
        }
        else if (current->type == PIPE)
        {
            break; // Stop processing args when we hit a pipe
        }
        
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

int count_args(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    while (current && current->type != PIPE)
    {
        if (current->type == CMD || current->type == ARG)
            count++;
        else if (current->type == TRUNC || current->type == APPEND ||
                 current->type == INPUT || current->type == HEREDOC)
        {
            current = current->next; // Skip the filename
            if (!current) break;
        }
        current = current->next;
    }
    return count;
}
