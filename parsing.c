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

/*char	*get_input(void)
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
}*/

void tokenize_input(char *input, char **args, int max_args)
{
	int		i;
	int		len;
	char	*start;
	char	*end;

	i = 0;
	start = input;
	while (*start && i < max_args)
	{
		while (ft_isspace(*start))
			start++;
		if (*start == '"')
		{
			start++;
			end = ft_strchr(start, '"');
		}
		else if (*start == '\'')
		{
			start++;
			end = ft_strchr(start, '\'');
		}
		else
		{
			end = start;
			while (*end && !ft_isspace(*end))
				end++;
		}
		if (end)
		{
			len = end - start;
			args[i] = (char *)malloc(len + 1);
			strncpy(args[i], start, len);
			args[i][len] = '\0';
			i++;
			if (*end)
				start = end + 1;
			else
				start = end;
		}
	}
	args[i] = NULL;
}

int	main(void)
{
	char	*input;
	char	*args[10];
	int	i;

	while(1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if(input[0] != NULL)
		{
			add_history(input);
			tokenize_input(input, args, 10);
		}
		i = 0;
		while (args[i])
		{
			printf("Token %d: %s\n", i, args[i]);
			i++;
		}
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(input);
	}
	rl_clear_history(); //free all the command history before terminating
	return (0);
}

	
