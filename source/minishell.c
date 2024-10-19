/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/18 15:46:48 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_last_status(t_exec_context *ctx, int status) 
{
	char	*new_status;
	int	i = 0;
	int	status_length;
	int	temp_status;
	
	status_length = 3; 
	if (status == 0)
		status_length += 1;
	else 
	{
		temp_status = status;
		while (temp_status > 0) 
		{
			temp_status /= 10;
			status_length++;
		}
	}
	new_status = (char *)malloc(status_length * sizeof(char)); 
	if (new_status == NULL) 
	{
		perror("malloc");
		return;
	}
	sprintf(new_status, "?=%d", status); 
	while (ctx->our_env[i]) 
	{
		if (strncmp(ctx->our_env[i], "?", 1) == 0)
		{
			free(ctx->our_env[i]); 
			ctx->our_env[i] = new_status; 
			return;
		}
		i++;
	}
	ctx->our_env = realloc(ctx->our_env, sizeof(char *) * (i + 2)); 
	if (ctx->our_env == NULL) 
	{
		perror("realloc");
		free(new_status); 
		return; 
	}
	ctx->our_env[i] = new_status; 
	ctx->our_env[i + 1] = NULL; 
}


void	minishell(t_exec_context *ctx)
{
	char	*input;
	t_token	*tokens;
	int status;
	
	status = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			rl_clear_history();
			break ;
		}
		if (input[0])
		{
			add_history(input);
			tokens = tokenize_inputs(input, 10);
			assign_token_types(tokens);
			if (tokens)
			{
				status = handle_tokens(tokens, ctx);
				update_last_status(ctx, status);
			}
			free_tokens(tokens);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_exec_context	ctx;
	(void)argc;
	(void)argv;
	
	ctx.pipe_fds = NULL;
	ctx.pipe_count = 0;
	ctx.current_index = 0;
	ctx.last_status = 0; 
	ctx.our_env = NULL;
	ctx.our_env = copy_environment(envp);
	if (!ctx.our_env)
	{
		write(2, "Failied to copy environment\n", 28);
		return (1);
	}
	setup_signal();
	minishell(&ctx);
	rl_clear_history();
	free_environment(ctx.our_env);
	return (0);
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

void	free_environment(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}


