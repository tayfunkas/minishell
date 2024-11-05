/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 01:22:59 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 06:48:26 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_pid_error(int *pipe_fd, char *delim)
{
	perror("fork");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	free(delim);
}

int	line_or_delim(char *delim, char *line)
{
	if (!line)
	{
		printf("minishell: warning: here-document delimited by");
		printf("end-of-file (wanted '%s')\n", delim);
		return (0);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

void	collect_heredoc_delimiters(t_tok *start, t_tok *end, char **delim)
{
	static t_tok	*current = NULL;

	if (current == NULL || current == end)
		current = start;
	while (current != end && current != NULL)
	{
		if (current->type == HEREDOC && current->next)
		{
			*delim = ft_strdup(current->next->str);
			if (*delim == NULL)
			{
				perror("Memory allocation failed");
				return ;
			}
			current = current->next->next;
			return ;
		}
		current = current->next;
	}
	*delim = NULL;
}

/*void	handle_heredoc_parent(int *pipe_fd, int *fd_in, pid_t pid, char *delim)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	free(delim);
	setup_signal();
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
}
*/