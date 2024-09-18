/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/09/17 16:49:05 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>

char	*get_input(void)
{
	char	*input;

	input = readline("\033[1;33mminishell>\033[0m ");
	if (input && *input)
		add_history(input);
	return (input);
}

void	parse_input(char *input)
{
	char	*token;
	token = strtok(input, " ");
	while (token != NULL)
	{
		printf("Token; %s\n", token);
		token = strtok(NULL, " ");
	}
}

int	main(void)
{
	char	*input;

	while(1)
	{
		input = get_input();
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if(*input)
			parse_input(input);
		free(input);
	}
	return (0);
}

	
