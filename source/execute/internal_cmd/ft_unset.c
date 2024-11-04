/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:00:55 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:36 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_key(char ***env, const char *key)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], key, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			break ;
		}
		i++;
	}
}

int	ft_unset(t_cmd *cmd, char ***env)
{
	char	*key;

	key = cmd->argv[1];
	if (!key)
		return (0);
	remove_env_key(env, key);
	return (0);
}
