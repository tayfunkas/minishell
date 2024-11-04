/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:09:54 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:26 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n_option(char *arg)
{
	int	j;

	j = 1;
	while (arg[j] == 'n')
		j++;
	return (arg[j] == '\0');
}

static void	print_args(char **args, int start_index, t_tok *tokens)
{
	while (args[start_index] != NULL)
	{
		write(STDOUT_FILENO, args[start_index], ft_strlen(args[start_index]));
		if (args[start_index + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		start_index++;
		tokens = tokens->next;
	}
}

int	ft_echo(char **args, t_tok *tokens)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while (args[i] != NULL && args[i][0] == '-')
	{
		if (check_n_option(args[i]))
		{
			newline = 0;
			i++;
		}
		else
			break ;
	}
	print_args(args, i, tokens);
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
