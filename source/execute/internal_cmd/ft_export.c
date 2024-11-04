/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:02:40 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 14:24:34 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	parse_key_value(char *key_value, char **key, char **value)
{
	char	*equals_sign;

	equals_sign = ft_strchr(key_value, '=');
	if (equals_sign == NULL)
	{
		*key = key_value;
		*value = NULL;
		return (1);
	}
	if (equals_sign == key_value)
	{
		return (-1);
	}
	*equals_sign = '\0';
	*key = key_value;
	*value = equals_sign + 1;
	return (0);
}

static int	add_single_env(char *key, char *value, char ***env,
	t_ctx *ctx)
{
	if (!is_valid_env_var_name(key))
	{
		write(2, "export: not a valid identifier\n", 31);
		return (1);
	}
	if (value == NULL)
		set_env(env, key, "", ctx);
	else
		set_env(env, key, value, ctx);
	return (0);
}

static int	add_new_env(t_cmd *cmd, char ***env, t_ctx *ctx)
{
	char	*key;
	char	*value;
	int		i;
	int		parse_result;

	i = 1;
	while (i < cmd->argc)
	{
		parse_result = parse_key_value(cmd->argv[i], &key, &value);
		if (parse_result == -1)
		{
			write(2, "export: not a valid identifier\n", 31);
			return (1);
		}
		if (add_single_env(key, value, env, ctx) != 0)
			return (1);
		if (parse_result == 0)
			*(value - 1) = '=';
		i++;
	}
	return (0);
}

int	ft_export(t_cmd *cmd, char ***env, t_ctx *ctx)
{
	if (cmd->argc < 2)
	{
		write(2, "export: not enough arguments\n", 29);
		return (1);
	}
	if (add_new_env(cmd, env, ctx) == 1)
		return (1);
	return (0);
}
