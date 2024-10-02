/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:22:39 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/02 14:37:41 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fork_and_exec(pid_t pid, t_token *cmd, int input_fd, int output_fd, char **envp)
{
	char	**args;
	int		token_count;

	token_count = count_tokens(cmd);
	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!args)
	{
		perror("malloc failed.\n");
		exit(EXIT_FAILURE);
	}
	prepare_args(cmd, token_count, args);
	if (pid == -1)
	{
		perror("fork failed.\n");
		free(args);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		if (execve(find_cmd_path(cmd->str), args, envp) == -1)
		{
			perror("execve failed.\n");
			free(args);
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_pipe(t_token *tokens, t_token *next_cmd, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed.\n");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	fork_and_exec(pid1, tokens, STDIN_FILENO, pipe_fd[1], envp);
	pid2 = fork();
	fork_and_exec(pid2, next_cmd, pipe_fd[0], STDOUT_FILENO, envp);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
}
