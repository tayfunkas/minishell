/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/25 13:21:51 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(t_token *tokens, t_exec_context *ctx)
{
	t_token *current = tokens;
	int pipe_count = 0;
	int cmd_count = 0;
	int redir_count = 0;
	int has_cmd = 0;

	if (!tokens)
		return (0);
	
	while (current)
	{
		if (current->type == PIPE)
		{
			pipe_count++;
			redir_count = 0;
			if (!has_cmd || !current->next || current->next->type == PIPE)
			{
				fprintf(stderr, "syntax error near unexpected token `|'\n");
				ctx->syntax_error = 1;
				return (0);
			}
			has_cmd = 0;
		}
		else if (current->type == CMD)
		{
			cmd_count++;
			redir_count = 0;
			has_cmd = 1;
		}
		else if (current->type == TRUNC || current->type == APPEND || 
			current->type == INPUT || current->type == HEREDOC)
		{
			redir_count++;
			if (redir_count > 1 || !current->next || 
				(current->next->type != ARG && current->next->type != CMD))
				{
					fprintf(stderr, "syntax error near unexpected token `%s'\n", current->str);
					ctx->syntax_error = 1;
					return (0);
				}
		}
		else if (current->type == ARG)
		{
			redir_count = 0;
			has_cmd = 1;  // Consider an ARG as a potential command
		}
		else if (current->type == END && pipe_count >= cmd_count)
		{
			fprintf(stderr, "syntax error: unexpected end of file\n");
			ctx->syntax_error = 1;
			return (0);
		}
		current = current->next;
	}
	if (pipe_count >= cmd_count && cmd_count > 0)
	{
		fprintf(stderr, "syntax error near unexpected token `|'\n");
		ctx->syntax_error = 1;
		return (0);
	}
	return (1);
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
			status = handle_tokens(tokens, ctx);
			update_last_status(ctx, status);
		}
		else
			update_last_status(ctx, 2);
	}
	free_tokens(tokens);
}

void	minishell(t_exec_context *ctx)
{
	char	*input;

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
