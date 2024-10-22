/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:17:17 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/22 14:03:17 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static char	*expand_var(char *token, char **our_env, char quote)
{
	char	*env_var;
	int		i;

	i = 0;
	if (token[0] == '$' && quote != '\'')
	{
		env_var = token + 1;
		while (our_env[i])
		{
			if (ft_strncmp(our_env[i], env_var, ft_strlen(env_var)) == 0
				&& our_env[i][ft_strlen(env_var)] == '=')
				return (ft_strdup(our_env[i] + ft_strlen(env_var) + 1));
			i++;
		}
		return (ft_strdup(""));
	}
	return (ft_strdup(token));
}*/

/*char *expand_var(char *token, t_exec_context *ctx)
{
    static char static_buffer[1024];
    char *result = static_buffer;
    size_t result_size = 1024;
    int i, j;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    i = 0;
    j = 0;
    while (token[i])
    {
        if (token[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            result[j++] = token[i++];
            continue;
        }
        else if (token[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            result[j++] = token[i++];
            continue;
        }

        if (token[i] == '$' && !in_single_quotes)
        {
            if (!token[i + 1] || token[i + 1] == ' ' || token[i + 1] == '"' || token[i + 1] == '\'')
            {
                result[j++] = token[i++];
            }
            else
            {
                i++;
                int k = i;
                while (token[k] && (isalnum(token[k]) || token[k] == '_' || token[k] == '?'))
                    k++;
                char *env_var = strndup(token + i, k - i);
                if (env_var)
                {
                    int env_index = 0;
                    while (ctx->our_env[env_index])
                    {
                        if (ft_strncmp(ctx->our_env[env_index], env_var, ft_strlen(env_var)) == 0
                            && ctx->our_env[env_index][ft_strlen(env_var)] == '=')
                        {
                            char *env_value = ctx->our_env[env_index] + ft_strlen(env_var) + 1;
                            size_t env_value_len = strlen(env_value);
                            if (j + env_value_len >= result_size)
                            {
                                if (result == static_buffer)
                                {
                                    result = malloc(result_size * 2);
                                    if (!result)
                                    {
                                        free(env_var);
                                        return NULL;
                                    }
                                    memcpy(result, static_buffer, j);
                                }
                                else
                                {
                                    char *new_result = realloc(result, result_size * 2);
                                    if (!new_result)
                                    {
                                        free(env_var);
                                        free(result);
                                        return NULL;
                                    }
                                    result = new_result;
                                }
                                result_size *= 2;
                            }
                            strcpy(result + j, env_value);
                            j += env_value_len;
                            break;
                        }
                        env_index++;
                    }
                    free(env_var);
                }
                i = k;
            }
        }
        else
        {
            result[j++] = token[i++];
        }

        if (j >= (int)result_size - 1)
        {
            if (result == static_buffer)
            {
                result = malloc(result_size * 2);
                if (!result)
                    return NULL;
                memcpy(result, static_buffer, j);
            }
            else
            {
                char *new_result = realloc(result, result_size * 2);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result_size *= 2;
        }
    }
    result[j] = '\0';

    if (result == static_buffer)
    {
        char *final_result = strdup(result);
        if (!final_result)
            return NULL;
        return final_result;
    }

    return result;
}*/

char *expand_var(char *token, t_exec_context *ctx)
{
    static char static_buffer[1024];
    char *result = static_buffer;
    size_t result_size = 1024;
    int i, j;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    i = 0;
    j = 0;
    while (token[i])
    {
        if (token[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            result[j++] = token[i++];
            // Copy everything within single quotes without expansion
            while (token[i] && (token[i] != '\'' || in_double_quotes))
            {
                result[j++] = token[i++];
                if (j >= (int)result_size - 1)
                {
                    if (result == static_buffer)
                    {
                        result = malloc(result_size * 2);
                        if (!result)
                            return NULL;
                        memcpy(result, static_buffer, j);
                    }
                    else
                    {
                        char *new_result = realloc(result, result_size * 2);
                        if (!new_result)
                        {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    result_size *= 2;
                }
            }
            if (token[i] == '\'')
            {
                result[j++] = token[i++];
            }
            continue;
        }
        else if (token[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            result[j++] = token[i++];
            continue;
        }

        if (token[i] == '$' && !in_single_quotes)
        {
            if (!token[i + 1] || token[i + 1] == ' ' || token[i + 1] == '"' || token[i + 1] == '\'')
            {
                result[j++] = token[i++];
            }
            else
            {
                i++;
                int k = i;
                while (token[k] && (isalnum(token[k]) || token[k] == '_' || token[k] == '?'))
                    k++;
                char *env_var = strndup(token + i, k - i);
                if (env_var)
                {
                    int env_index = 0;
                    while (ctx->our_env[env_index])
                    {
                        if (ft_strncmp(ctx->our_env[env_index], env_var, ft_strlen(env_var)) == 0
                            && ctx->our_env[env_index][ft_strlen(env_var)] == '=')
                        {
                            char *env_value = ctx->our_env[env_index] + ft_strlen(env_var) + 1;
                            size_t env_value_len = strlen(env_value);
                            if (j + env_value_len >= result_size)
                            {
                                if (result == static_buffer)
                                {
                                    result = malloc(result_size * 2);
                                    if (!result)
                                    {
                                        free(env_var);
                                        return NULL;
                                    }
                                    memcpy(result, static_buffer, j);
                                }
                                else
                                {
                                    char *new_result = realloc(result, result_size * 2);
                                    if (!new_result)
                                    {
                                        free(env_var);
                                        free(result);
                                        return NULL;
                                    }
                                    result = new_result;
                                }
                                result_size *= 2;
                            }
                            strcpy(result + j, env_value);
                            j += env_value_len;
                            break;
                        }
                        env_index++;
                    }
                    free(env_var);
                }
                i = k;
            }
        }
        else
        {
            result[j++] = token[i++];
        }

        if (j >= (int)result_size - 1)
        {
            if (result == static_buffer)
            {
                result = malloc(result_size * 2);
                if (!result)
                    return NULL;
                memcpy(result, static_buffer, j);
            }
            else
            {
                char *new_result = realloc(result, result_size * 2);
                if (!new_result)
                {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result_size *= 2;
        }
    }
    result[j] = '\0';

    if (result == static_buffer)
    {
        char *final_result = strdup(result);
        if (!final_result)
            return NULL;
        return final_result;
    }

    return result;
}

void	prep_args(t_token *tokens, int token_count, char **args, t_exec_context *ctx)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < token_count)
	{
		if (current->type == CMD || current->type == ARG)
		{
			args[i] = expand_var(current->str, ctx);
			i++;
		}
		else if (current->type == TRUNC || current->type == APPEND
			|| current->type == INPUT || current->type == HEREDOC)
			current = current->next;
		current = current->next;
	}
	args[i] = NULL;
}

char	**allocate_args(int token_count)
{
	char	**args;

	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (args == NULL)
		perror("malloc failed");
	return (args);
}
