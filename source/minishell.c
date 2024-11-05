/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:23:05 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 21:27:23 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	when_signals(t_ctx *ctx)
{
	if (g_signal == 2)
	{
		set_env(&(ctx->our_env), "?", "130", ctx);
		g_signal = 0;
	}
	if (g_signal == 1)
	{
		set_env(&(ctx->our_env), "?", "1", ctx);
		g_signal = 0;
	}
}

static void	run_tokens(t_tok *tokens, t_ctx *ctx)
{
	int	status;

	status = 0;
	expand_tokens(tokens, ctx);
	assign_token_types(tokens);
	if (check_syntax(tokens))
	{
		signal(SIGINT, SIG_IGN);
		status = handle_tokens(tokens, ctx);
		setup_signal();
		update_last_status(ctx, status);
	}
	else
	{
		write(2, "minishell: syntax error\n", 25);
		update_last_status(ctx, 2);
	}
}

static void	run_minishell(char *input, t_ctx *ctx)
{
	t_tok	*tokens;

	when_signals(ctx);
	if (!check_invalid_sequences(input))
	{
		write(2, "minishell: syntax error\n", 25);
		update_last_status(ctx, 2);
		return ;
	}
	tokens = tokenize_inputs(input, 20);
	if (tokens)
		run_tokens(tokens, ctx);
}

static void	minishell(t_ctx *ctx)
{
	char	*input;

	input = NULL;
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
	t_ctx	ctx;

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
