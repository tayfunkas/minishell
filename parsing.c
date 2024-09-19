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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	tokenize_input(char *input, char **args, int max_args)
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
			ft_strncpy(args[i], start, len);
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

void	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
			perror("minishell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("minishell");
	else
	{
		while (waitpid(pid, &status, WUNTRACED) != -1 && !WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

int	main(void)
{
	char	*input;
	char	*args[10];
	int		i;

	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input[0])
		{
			add_history(input);
			tokenize_input(input, args, 10);
			if (args[0] != NULL)
				execute_command(args);
		}
		i = 0;
		//while (args[i])
		//{
		//	printf("Token %d: %s\n", i, args[i]);
		//	i++;
		//}
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}


	
