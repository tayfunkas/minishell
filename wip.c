/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wip.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:41:26 by kyukang           #+#    #+#             */
/*   Updated: 2024/09/25 17:41:42 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_token *tokens)
{
	pid_t	pid;
	int	status;
	char	*args[10]; // Increased size to handle more arguments
	int	i;

	status = 0;
	i = 0;
	while (tokens && tokens->str && i < 10 && (tokens->type == CMD || tokens->type == ARG))
	{
		args[i++] = tokens->str;
		tokens = tokens->next;
	}
	args[i] = NULL;
	if (i == 0)
		return;
	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
			perror("minishell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	perror("minishell");
	else
	{
		while (waitpid(pid, &status, WUNTRACED) != -1 && !WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

/*
void	handle_tokens(t_token *tokens)
{
	t_token	*current = tokens;

	while (current != NULL)
	{
		if (current->type == PIPE)
			handle_pipe(current);
		else if (current->type == TRUNC)
			handle_redirection(current);
		else if (current->type == APPEND)
			handle_redirection(current);
		else if (current->type == INPUT)
			handle_redirection(current);
		else if (current->type == CMD || current->type == ARG)
			handle_command(current);
		else if (current->type == END)
			execute_pipeline(tokens);
		current = current->next;
	}
}
*/