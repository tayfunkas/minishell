/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/29 18:20:32 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	ctx.syntax_error = 0;
	ctx.our_env = copy_environment(envp);
	if (!ctx.our_env)
	{
		write(2, "Failied to copy environment\n", 28);
		return (1);
	}
	minishell(&ctx);
	rl_clear_history();
	free_environment(ctx.our_env);
	return (0);
}

static void	run_minishell(char *input, t_exec_context *ctx)
{
	t_token	*tokens;
	int		status;

	tokens = tokenize_inputs(input, 10);
	if (tokens)
	{
		expand_tokens(tokens, ctx);
		assign_token_types(tokens);
		if (check_syntax(tokens, ctx))
		{
			signal(SIGINT, SIG_IGN);
			status = handle_tokens(tokens, ctx);
			setup_signal();
			update_last_status(ctx, status);
		}
		else
			update_last_status(ctx, 2);
	}
}

void	minishell(t_exec_context *ctx)
{
	char	*input;

	g_signal = 0;
	setup_signal();
	initialize_exit_status(ctx);
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
			run_minishell(input, ctx);
		}
		free(input);
	}
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
