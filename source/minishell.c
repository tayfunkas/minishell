/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:23:05 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 22:17:28 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_minishell(char *input, t_exec_context *ctx)
{
	t_token	*tokens;
	int		status;

	status = 0;
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

static void	minishell(t_exec_context *ctx)
{
	char	*input = NULL;

	g_signal = 0;
	setup_signal();
	initialize_exit_status(ctx);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit!\n");
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
	free_context(&ctx);
	return (0);
}
