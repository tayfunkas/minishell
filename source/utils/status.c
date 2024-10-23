/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:36:41 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/23 13:59:55 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_status_str(int status)
{
	char	*status_str;
	char	*new_status;
	size_t	total_len;

	status_str = ft_itoa(status);
	if (status_str == NULL)
	{
		perror("ft_itoa");
		return (NULL);
	}
	total_len = 2 + ft_strlen(status_str) + 1;
	new_status = (char *)malloc(sizeof(char) * total_len);
	if (new_status == NULL)
	{
		perror("malloc");
		free(status_str);
		return (NULL);
	}
	ft_strcpy(new_status, "?=");
	ft_strcat(new_status, status_str);
	free(status_str);
	return (new_status);
}

static int	update_existing_status(t_exec_context *ctx, char *new_status)
{
	int	i;

	i = 0;
	while (ctx->our_env[i])
	{
		if (ft_strncmp(ctx->our_env[i], "?", 1) == 0)
		{
			free(ctx->our_env[i]);
			ctx->our_env[i] = new_status;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	add_new_status(t_exec_context *ctx, char *new_status)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	while (ctx->our_env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	if (new_env == NULL)
	{
		perror("malloc");
		free(new_status);
		return (0);
	}
	j = 0;
	while (j < i)
	{
		new_env[j] = ctx->our_env[j];
		j++;
	}
	new_env[j] = new_status;
	new_env[j + 1] = NULL;
	free(ctx->our_env);
	ctx->our_env = new_env;
	return (1);
}

void	update_last_status(t_exec_context *ctx, int status)
{
	char	*new_status;

	new_status = create_status_str(status);
	if (new_status == NULL)
		return ;
	if (!update_existing_status(ctx, new_status))
	{
		if (!add_new_status(ctx, new_status))
		{
			perror("Failed to add new status");
			free(new_status);
		}
	}
}

int	initialize_exit_status(t_exec_context *ctx)
{
	char	*initial_status;

	initial_status = ft_strdup("?=0");
	if (initial_status == NULL)
	{
		write(2, "malloc\n", 7);
		return (0);
	}
	ft_strcpy(initial_status, "?=0");
	if (!add_new_status(ctx, initial_status))
	{
		write(2, "Failed to add initial status\n", 29);
		free(initial_status);
		return (0);
	}
	return (1);
}
