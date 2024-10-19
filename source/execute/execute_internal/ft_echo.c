/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:09:54 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/19 18:43:09 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	bool	newline; 
	bool	first;
	int		i;
	char	*arg;

	newline = 1;
	i = 1; 
	while (args[i] != NULL && strncmp(args[i], "-n", 2) == 0)
	{
		arg = args[i] + 1;
		while (*arg == 'n')
			arg++;
		if (*arg == '\0')
		{
			newline = 0;
			i++;
		}
		else
			break ;
	}
	first = 1;
	while (args[i] != NULL)
	{
		if (!first)
			printf(" ");
		printf("%s", args[i]);
		first = 0;
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
