/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:02:40 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/28 15:38:59 by kyukang          ###   ########.fr       */
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

static int	extend_env(char ***env, char *new_var, t_exec_context *ctx)
{
	char	**new_env;
	int		env_size;
	int		i;

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
	i = 0;
	while (i < env_size)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[env_size] = new_var;
	new_env[env_size + 1] = NULL;
	if (*env != NULL)
		free(*env);
	ctx->our_env = new_env;
	return (0);
}

int	set_env(char ***env, const char *name, const char *value,
	t_exec_context *ctx)
{
	int		i;
	int		len;
	char	*new_var;
	int		j;

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
	if (extend_env(env, new_var, ctx) == -1)
	{
		free(new_var);
		return (-1);
	}
	j = 0;
	return (0);
}

static int	is_valid_env_var_name(const char *name)
{
	int	i;

	i = 0;
	if (name == NULL || name[0] == '\0')
		return (0);
	if (!isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i] != '\0')
	{
		if (!isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_command *cmd, char ***env, t_exec_context *ctx)
{
	char	*key_value;
	char	*equals_sign;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	if (cmd->argc < 2)
	{
		write(2, "export: not enough arguments\n", 29);
		return (1);
	}
	while (i < cmd->argc)
	{
		key_value = cmd->argv[i];
		equals_sign = ft_strchr(key_value, '=');
		if (equals_sign == NULL)
		{
			if (!is_valid_env_var_name(key_value))
			{
				write(2, "export: not a valid identifier\n", 31);
				return (1);
			}
			set_env(env, key_value, "", ctx);
			i++;
			continue ;
		}
		if (equals_sign == key_value)
		{
			write(2, "export: not a valid identifier\n", 31);
			return (1);
		}
		*equals_sign = '\0';
		key = key_value;
		value = equals_sign + 1;
		if (!is_valid_env_var_name(key))
		{
			write(2, "export: not a valid identifier\n", 31);
			*equals_sign = '=';
			return (1);
		}
		set_env(env, key, value, ctx);
		*equals_sign = '=';
		i++;
	}
	return (0);
}
