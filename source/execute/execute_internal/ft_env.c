/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:03:47 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/23 14:52:20 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env, t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->argc > 1)
	{
		write(2, "env: no such file or directory\n", 32);
		return (127);
	}	
	if (env == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return (1);
	}
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
