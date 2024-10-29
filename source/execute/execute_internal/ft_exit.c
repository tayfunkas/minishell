/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:00:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/29 14:16:08 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args, t_exec_context *ctx)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (args[1] == NULL)
	{
		free_context(ctx);
		exit(0);
	}
	if (!ft_isdigit_str(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		free_context(ctx);
		exit(2);
	}
	if (args[2] != NULL)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	status = ft_atoi(args[1]) % 256;
	free_context(ctx);
	exit(status);
}
