/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/02 14:42:47 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	is_pipe_redir(t_token *current, char **our_env)
{
	if (current->type == PIPE)
	{
		handle_pipe(current, current->next, our_env);
		return ;
	}
	if (current->type == TRUNC || current->type == APPEND
		|| current->type == INPUT)
	{
		//handle_redirection(tokens, current->next->str, current->type);
		return ;
	}
}*/

static int	is_external_command(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = NULL;
	i = 0;
	if (ft_strcmp(cmd, "env") == 0)
		return (0);
	while (our_env[i])
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (1);
		}
		i++;
	}
	free_split(paths);
	return (0);
}

static int	is_internal_command(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void handle_tokens(t_token *tokens, char **our_env)
{
    t_token *current = tokens;
    t_token *pipe_token = NULL;
    int pipefd[2];
    pid_t pid1, pid2;

    // Find the pipe token, if it exists
    while (current != NULL)
    {
        if (current->type == PIPE)
        {
            pipe_token = current;
            break;
        }
        current = current->next;
    }

    if (pipe_token)
    {
        // We have a pipe, so we need to handle two commands
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
        }

        pid1 = fork();
        if (pid1 == 0)
        {
            // First child process (left side of pipe)
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execute_command(tokens, pipe_token, our_env);
            exit(EXIT_SUCCESS);
        }

        pid2 = fork();
        if (pid2 == 0)
        {
            // Second child process (right side of pipe)
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execute_command(pipe_token->next, NULL, our_env);
            exit(EXIT_SUCCESS);
        }

        // Parent process
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
    else
    {
        // No pipe, execute single command
        execute_command(tokens, NULL, our_env);
    }
}

void execute_command(t_token *start, t_token *end, char **our_env)
{
    if (start && start->type == CMD)
    {
        if (is_external_command(start->str, our_env))
        {
            printf("(external)\n");
            execute_external_command(start, count_tokens_until(start, end), our_env);
        }
        else if (is_internal_command(start->str))
        {
            t_command *cmd = init_internal_command(start, our_env);
            if (!cmd)
                return;
            printf("(internal)\n");
            execute_internal_commands(cmd, &our_env);
            free_command(cmd);
        }
    }
}

int count_tokens_until(t_token *start, t_token *end)
{
    int count = 0;
    t_token *current = start;
    while (current != end && current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
