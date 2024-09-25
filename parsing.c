/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/09/17 16:49:05 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *tokenize_input(char *input, int max_args)
{
	t_token	*head = NULL;
	t_token	*current = NULL;
	int 	i;
	int	inside_quotes;
	char	*start = input;
	char	*end;

	i = 0;
	inside_quotes = 0;
	while (*start && i < max_args)
	{
		while (ft_isspace(*start))
			start++;
		if (*start == '"' || *start == '\'')
		{
			inside_quotes = 1;
			char quote = *start;
			start++;
			end = ft_strchr(start, quote);
		}
		else
		{
			inside_quotes = 0;
			end = start;
			while (*end && !ft_isspace(*end))
			end++;
		}
		if (end)
		{
			t_token	*new_token = (t_token *)malloc(sizeof(t_token));
			int	len = end - start;
			new_token->str = (char *)malloc(len + 1);
			ft_strncpy(new_token->str, start, len);
			new_token->str[len] = '\0';
			new_token->flag = inside_quotes;
			new_token->next = NULL;
			new_token->prev = current;
			if (current)
				current->next = new_token;
			else
				head = new_token;
			current = new_token;
			i++;
			if (*end)
				start = end + 1;
			else
				start = end;
		}
	}
	return head;
}

void	assign_token_types(t_token *tokens)
{
	t_token	*current;
	
	current = tokens;
	while (current)
	{
		type_arg(current);
		current = current->next;
	}
}

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

void	type_arg(t_token *token)
{
	if (ft_strcmp(token->str, "") == 0)
		token->type = EMPTY;
	else if (ft_strcmp(token->str, ">") == 0 && token->flag == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->str, ">>") == 0 && token->flag == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->str, "<") == 0 && token->flag == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->str, "|") == 0 && token->flag == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0 && token->flag == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type= ARG;
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

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	
	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
}

int	main(void)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if (input[0])
		{
			add_history(input);
			tokens = tokenize_input(input, 10);
			assign_token_types(tokens);
			if (tokens)
				execute_command(tokens);
			free_tokens(tokens);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
