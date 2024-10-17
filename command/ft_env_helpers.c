/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:44:11 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/16 17:02:29 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char **env, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}

static int	find_new_var(char ***env, const char *name, int len)
{
	int	i;

	i = 0;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], name, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			return (i);
		}
		i++;
	}
	return (i);
}

static char	*create_env_var(const char *name, const char *value)
{
	char	*new_var;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!new_var)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	return (new_var);
}

static int	extend_env(char ***env, int i, char *new_var)
{
	char	**new_env;

	if ((*env)[i] == NULL)
	{
		new_env = realloc(*env, sizeof(char *) * (i + 2));
		if (!new_env)
		{
			free(new_var);
			perror("realloc");
			return (-1);
		}
		*env = new_env;
		(*env)[i + 1] = NULL;
	}
	(*env)[i] = new_var;
	return (0);
}

void	set_env(char ***env, const char *name, const char *value)
{
	int		i;
	int		len;
	char	*new_var;

	if (!env || !*env || !name || !value)
	{
		printf("set_env: Invalid arguments\n");
		return ;
	}
	len = ft_strlen(name);
	i = find_new_var(env, name, len);
	new_var = create_env_var(name, value);
	if (!new_var)
		return ;
	if (extend_env(env, i, new_var) == -1)
		return ;
}
