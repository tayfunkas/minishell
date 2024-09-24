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

void	tokenize_input(char *input, char **args, int max_args, t_token *tokens)
{
	int		i;
	int		len;
	char	*start;
	char	*end;
	int		inside_quotes;

	i = 0;
	inside_quotes = 0;
	start = input;
	while (*start && i < max_args)
	{
		while (ft_isspace(*start))
			start++;
		if (*start == '"')
		{
			inside_quotes = 1;
			start++;
			end = ft_strchr(start, '"');
		}
		else if (*start == '\'')
		{
			inside_quotes = 1;
			start++;
			end = ft_strchr(start, '\'');
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
			len = end - start;
			args[i] = (char *)malloc(len + 1);
			ft_strncpy(args[i], start, len);
			args[i][len] = '\0';
			tokens[i].str = args[i];
			tokens[i].flag = inside_quotes;
			i++;
			if (*end)
				start = end + 1;
			else
				start = end;
		}
	}
	args[i] = NULL;
}

void	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	status = 0;
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

int	main(void)
{
	char	*input;
	char	*args[10];
	t_token	tokens[10];
	int		i;

	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input[0])
		{
			add_history(input);
			tokenize_input(input, args, 10, tokens);
			if (args[0] != NULL)
				execute_command(args);
		}
		i = 0;
		//while (args[i])
		//{
		//	printf("Token %d: %s\n", i, args[i]);
		//	i++;
		//}
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}


	
