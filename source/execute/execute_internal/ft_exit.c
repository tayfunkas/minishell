/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:00:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:00:41 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1] == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (!ft_isdigit_str(args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	if (args[2] != NULL)
	{
		write(1, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	status = ft_atoi(args[1]);
	printf("exit\n");
	exit(status);
}
