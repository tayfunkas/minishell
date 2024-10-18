/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:02:40 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/17 16:45:55 by kyukang          ###   ########.fr       */
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

int	set_env(char ***env, const char *name, const char *value)
{
	int		i;
	int		len;
	char	*new_var;

	if (!env || !*env || !name || !value)
	{
		printf("set_env: Invalid arguments\n");
		return (-1);
	}
	len = ft_strlen(name);
	i = find_new_var(env, name, len);
	new_var = create_env_var(name, value);
	if (!new_var)
		return (-1);
	if (extend_env(env, i, new_var) == -1)
	{
		free(new_var);
		return (-1);
	}
	return (0);
}

int	ft_export(t_command *cmd, char ***env)
{
	char	*key_value;
	char	*equals_sign;
	char	*key;
	char	*value;

	if (cmd->argc < 2)
	{
		write(2, "export: not enough arguments\n", 29);
		return (1);
	}
	key_value = cmd->argv[1];
	equals_sign = ft_strchr(key_value, '=');
	if (equals_sign)
	{
		*equals_sign = '\0';
		key = key_value;
		value = equals_sign + 1;
		set_env(env, key, value);
		*equals_sign = '=';
	}
	else
		set_env(env, key_value, "");
	return (0);
}
