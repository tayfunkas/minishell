/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:22:39 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/02 15:17:54 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fork_and_exec(pid_t pid, t_token *cmd, int input_fd, int output_fd, char **envp)
{
	char	**args;
	int		token_count;
	char	*cmd_path;

	token_count = count_tokens(cmd);
	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!args)
	{
		perror("malloc failed.\n");
		exit(EXIT_FAILURE);
	}
	prepare_args(cmd, token_count, args, envp);
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
			printf("Redirecting stdin to pipe (fd %d).\n", input_fd);
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		if (output_fd != STDOUT_FILENO)
		{
			printf("Redirecting stdout to pipe (fd %d).\n", output_fd);
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
		}
		printf("Finding path for command: %s\n", cmd->str);
		cmd_path = find_cmd_path(cmd->str, envp);
		if (!cmd_path)
		{
			printf("Command path not found for %s\n", cmd->str);
			free(args);
			exit(EXIT_FAILURE);
		}
		printf("Executing command: %s\n", cmd->str);
		printf("Command path: %s\n", cmd_path);
		for (int i = 0; args[i] != NULL; i++)
			printf("args[%d]: %s\n", i, args[i]);
		if (execve(cmd_path, args, envp) == -1)
		{
			perror("execve failed.\n");
			free(cmd_path);
			free(args);
			exit(EXIT_FAILURE);
		}
	}
	free(args);
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
	if (pid1 == 0)
	{
		printf("Forked first cmd process \n");
		close(pipe_fd[0]);
		fork_and_exec(pid1, tokens, STDIN_FILENO, pipe_fd[1], envp);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		printf("Forked second cmd process \n");
		close(pipe_fd[1]);
		fork_and_exec(pid2, next_cmd, pipe_fd[0], STDOUT_FILENO, envp);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
