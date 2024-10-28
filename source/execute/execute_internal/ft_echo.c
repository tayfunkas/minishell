/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:09:54 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/28 15:38:31 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	newline;
	int	i;
	int	j;

	newline = 1;
	i = 1;
	while (args[i] != NULL && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			newline = 0;
			i++;
		}
		else
			break ;
	}
	while (args[i] != NULL)
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
