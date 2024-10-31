/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:44:37 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/28 17:53:43 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	if_pipe(t_token *current, t_token *tokens, t_exec_context *ctx)
{
	if (current->type == PIPE)
	{
		tokens->pipe_count++;
		tokens->redir_count = 0;
		if (!tokens->cmd_flag || !current->next || current->next->type == PIPE)
		{
			write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 39);
			ctx->syntax_error = 1;
			return (0);
		}
		tokens->cmd_flag = 0;
	}
	return (1);
}

static int	if_redir(t_token *current, t_token *tokens, t_exec_context *ctx)
{
	if (current->type == TRUNC || current->type == APPEND
		|| current->type == INPUT || current->type == HEREDOC)
	{
		tokens->redir_count++;
		if (tokens->redir_count > 1 || !current->next
			|| (current->next->type != ARG && current->next->type != CMD))
		{
			write(STDERR_FILENO, "syntax error near unexpected token ", 35);
			printf("'%s'\n", current->str);
			ctx->syntax_error = 1;
			return (0);
		}
	}
	return (1);
}

static int	if_end(t_token *current, t_token *tokens, t_exec_context *ctx)
{
	if (current->type == END && tokens->pipe_count >= tokens->cmd_count)
	{
		write(STDERR_FILENO, "syntax error: unexpected end of file\n", 37);
		ctx->syntax_error = 1;
		return (0);
	}
	return (1);
}

static int	check_token_type(t_token *current, t_token *tokens,
	t_exec_context *ctx)
{
	while (current)
	{
		if (!if_pipe(current, tokens, ctx))
			return (0);
		else if (current->type == CMD)
		{
			tokens->cmd_count++;
			tokens->redir_count = 0;
			tokens->cmd_flag = 1;
		}
		else if (!if_redir(current, tokens, ctx))
			return (0);
		else if (current->type == ARG)
		{
			tokens->redir_count = 0;
			tokens->cmd_flag = 1;
		}
		else if (!if_end(current, tokens, ctx))
			return (0);
		current = current->next;
	}
	return (1);
}

int	check_syntax(t_token *tokens, t_exec_context *ctx)
{
	t_token	*current;

	current = tokens;
	tokens->pipe_count = 0;
	tokens->cmd_count = 0;
	tokens->redir_count = 0;
	tokens->cmd_flag = 0;
	if (!tokens)
		return (0);
	if (check_token_type(current, tokens, ctx) == 0)
		return (0);
	if (tokens->pipe_count >= tokens->cmd_count && tokens->cmd_count > 0)
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 39);
		ctx->syntax_error = 1;
		return (0);
	}
	return (1);
}
