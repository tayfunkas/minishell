/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:17:17 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:18:04 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_var(char *token, char **our_env, char quote)
{
	char	*env_var;
	int		i;

	i = 0;
	if (token[0] == '$' && quote != '\'')
	{
		env_var = token + 1;
		while (our_env[i])
		{
			if (ft_strncmp(our_env[i], env_var, ft_strlen(env_var)) == 0
				&& our_env[i][ft_strlen(env_var)] == '=')
				return (ft_strdup(our_env[i] + ft_strlen(env_var) + 1));
			i++;
		}
		return (ft_strdup(""));
	}
	return (ft_strdup(token));
}

void	prep_args(t_token *tokens, int token_count, char **args, char **our_env)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < token_count)
	{
		if (current->type == CMD || current->type == ARG)
		{
			args[i] = expand_var(current->str, our_env, current->quote);
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
