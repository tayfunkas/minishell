/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:03:47 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:03:53 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **env)
{
	int	i;

	i = 0;
	if (env == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return ;
	}
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}