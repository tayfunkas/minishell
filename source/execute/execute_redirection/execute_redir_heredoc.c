/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:45:39 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/29 21:03:14 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	errmsg_if_no_line(char *line, char *delimiter)
{
	if (!line)
	{
		printf("minishell: warning: here-document at line ");
		printf("delimited by end-of-file (wanted '%s')\n", delimiter);
	}
}

static void heredoc_input(int *pipe_fd, char **delimiters, int delimiter_count)
{
    char *line;
    int current_delimiter = 0;

    signal(SIGINT, child_sigint_handler);
    close(pipe_fd[0]);
    while (current_delimiter < delimiter_count)
    {
        while (1)
        {
            line = readline("> ");
            if (!line || ft_strcmp(line, delimiters[current_delimiter]) == 0)
            {
                errmsg_if_no_line(line, delimiters[current_delimiter]);
                free(line);
                break;
            }
            if (write(pipe_fd[1], line, ft_strlen(line)) == -1
                || write(pipe_fd[1], "\n", 1) == -1)
            {
                perror("write to pipe");
                free(line);
                exit(EXIT_FAILURE);
            }
            free(line);
        }
        current_delimiter++;
    }
    close(pipe_fd[1]);
    exit(EXIT_SUCCESS);
}

static void	heredoc_wait(int *pipe_fd, int *fd_in, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	setup_signal();
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
}

static void	recursive_heredoc(t_token *current, int *fd_in, char **delimiters, int *delimiter_count)
{
    if (current == NULL || current->type != HEREDOC)
        return;

    delimiters[*delimiter_count] = current->next->str;
    (*delimiter_count)++;

    recursive_heredoc(current->next->next, fd_in, delimiters, delimiter_count);
}

void execute_redir_heredoc(t_token *current, int *fd_in)
{
    int pipe_fd[2];
    char *delimiters[100];
    int delimiter_count = 0;
    pid_t pid;

    recursive_heredoc(current, fd_in, delimiters, &delimiter_count);

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
        heredoc_input(pipe_fd, delimiters, delimiter_count);
    else
        heredoc_wait(pipe_fd, fd_in, pid);
}
