/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/09/25 18:17:47 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if (input[0])
		{
			add_history(input);
			tokens = tokenize_input(input, 10);
			assign_token_types(tokens);
			if (tokens)
				handle_tokens(tokens);
			free_tokens(tokens);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
