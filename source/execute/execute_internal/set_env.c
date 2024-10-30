/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:54:49 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/30 13:36:18 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_new_var(char ***env, const char *name, int len)
{
	int	i;

	i = 0;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], name, len) == 0 && (*env)[i][len] == '=')
			return (i);
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

static int	extend_env(char ***env, char *new_var, t_exec_context *ctx)
{
	char	**new_env;
	int		env_size;

	env_size = 0;
	while ((*env)[env_size])
		env_size++;
	new_env = malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
	{
		free(new_var);
		perror("malloc failed");
		return (-1);
	}
	ft_memcpy(new_env, *env, sizeof(char *) * env_size);
	new_env[env_size] = new_var;
	new_env[env_size + 1] = NULL;
	free(*env);
	*env = new_env;
	ctx->our_env = new_env;
	return (0);
}

static int	check_env(char ***env, const char *name, const char *value)
{
	if (!env || !*env || !name || !value)
	{
		printf("set_env: Invalid arguments\n");
		return (-1);
	}
	return (0);
}

int	set_env(char ***env, const char *name, const char *value, 
	t_exec_context *ctx)
{
	int		i;
	int		len;
	char	*new_var;

	check_env(env, name, value);
	len = ft_strlen(name);
	i = find_new_var(env, name, len);
	new_var = create_env_var(name, value);
	if (!new_var)
		return (-1);
	if ((*env)[i] != NULL)
	{
		free((*env)[i]);
		(*env)[i] = new_var;
	}
	else
	{
		if (extend_env(env, new_var, ctx) == -1)
		{
			free(new_var);
			return (-1);
		}
	}
	return (0);
}
