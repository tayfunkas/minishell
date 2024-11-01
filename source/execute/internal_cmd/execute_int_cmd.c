/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_int_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:52:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/01 19:31:31 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_internal_c(t_command *cmd, char ***env, t_exec_context *ctx,
	t_token *tokens)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd, cmd->argv[1], env, ctx));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd, env, ctx));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd, env));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*env, cmd));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv, ctx, tokens, cmd));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd->argv));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd());
	else
		return (127);
}

static void	dup_and_close(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

static void	setup_cmd_fds(t_command *cmd, t_token *start, t_token *end, t_exec_context *ctx)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	(void)start;
	(void)end;
	(void)ctx;
	setup_redir(start, end, &cmd->fd_in, &cmd->fd_out);
}

int	execute_internal_cmd(t_exec_context *ctx, t_token *start, t_token *end, t_token *tokens)
{
	t_command	*cmd;
	int			status = 0;
	int			parent_in = 0;
	int			parent_out = 0;
	pid_t		pid;

	(void)end;
	cmd = init_internal_cmd(start);
	if (!cmd)
		return (1);
	parent_in = dup(STDIN_FILENO);
	parent_out = dup(STDOUT_FILENO);
	if (ctx->pipe_count != 0)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			free_command(cmd);
			return (1);
		}
		else if (pid == 0)
		{
			setup_child_pipes(ctx);
			setup_cmd_fds(cmd, start, end, ctx);
			if (cmd->fd_in != STDIN_FILENO)
			{
				dup2(cmd->fd_in, STDIN_FILENO);
				close(cmd->fd_in);
			}
			if (cmd->fd_out != STDOUT_FILENO)
			{
				dup2(cmd->fd_out, STDOUT_FILENO);
				close(cmd->fd_out);
			}
			status = check_internal_c(cmd, &ctx->our_env, ctx, tokens);
			free_command(cmd);
			return (status);
		}
		else
			waitpid(pid, &status, 0);
	}
	else
	{
		setup_cmd_fds(cmd, start, end, ctx);
		if (cmd->fd_in != STDIN_FILENO)
		{
			dup2(cmd->fd_in, STDIN_FILENO);
			close(cmd->fd_in);
		}
		if (cmd->fd_out != STDOUT_FILENO)
		{
			dup2(cmd->fd_out, STDOUT_FILENO);
			close(cmd->fd_out);
		}
		status = check_internal_c(cmd, &ctx->our_env, ctx, tokens);
	}
	dup_and_close(parent_in, parent_out);
	free_command(cmd);
	return (status);
}
