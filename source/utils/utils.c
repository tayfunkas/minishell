/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:29 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 14:43:04 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_error(const char *message_prefix, const char *command)
{
	write(STDERR_FILENO, message_prefix, ft_strlen(message_prefix));
	write(STDERR_FILENO, command, ft_strlen(command));
	write(STDERR_FILENO, "\n", 1);
}

char	**copy_environment(char **envp)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	i = 0;
	while (envp[count] != NULL)
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i++])
		{
			while (i-- >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}
	new_env[count] = NULL;
	return (new_env);
}
