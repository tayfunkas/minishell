/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:58 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/11 15:13:34 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *path, char ***env)
{
	char	current_dir[1024];
	char	new_dir[1024];
	char	*home;
	char	*oldpwd;

	if (env == NULL || *env == NULL) 
	{
		printf("Error: env is NULL\n");
		return ;
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	home = get_env_value(*env, "HOME");
	if (path == NULL || path[0] == '\0' || ft_strcmp(path, "~") == 0)
	{
		path = home;
		if (path == NULL)
		{
			printf("Error: HOME not set\n");
			return ;
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		oldpwd = get_env_value(*env, "OLDPWD");
		if (oldpwd == NULL)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return ;
		}
		path = oldpwd;
		printf("%s\n", path);
	}
	else if (path[0] == '~' && path[1] == '/')
	{
		if (home == NULL)
		{
			printf("Error: HOME not set\n");
			return ;
		}
		ft_strcpy(new_dir, home);
		ft_strcat(new_dir, path + 1);
		path = new_dir;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	if (getcwd(new_dir, sizeof(new_dir)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	set_env(env, "OLDPWD", current_dir);
	set_env(env, "PWD", new_dir);
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	ft_echo(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	if (args[1] != NULL && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
		{
			printf(" ");
		}
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_export(t_command *cmd, char ***env)
{
	char	*key_value;
	char	*equals_sign;
	char	*key;
	char	*value; 
	
	key_value = cmd->argv[1];
	equals_sign = ft_strchr(key_value, '=');
	
	if (cmd->argc < 2)
	{
		write(2, "export: not enough arguments\n", 29);
		return ;
	}
	if (equals_sign)
	{
		*equals_sign = '\0';
		key = key_value;
		value = equals_sign + 1;
		set_env(env, key, value);
		*equals_sign = '=';
	}
	else
		set_env(env, key_value, "");
}

void	set_env(char ***env, const char *name, const char *value)
{
	int	i;
	int	len;
	char	*new_var;
    
	if (!env || !*env || !name || !value)
	{
		fprintf(stderr, "set_env: Invalid arguments\n");
		return;
	}
	i = 0;
	len = ft_strlen(name);
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], name, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			break;
		}
		i++;
	}
    
    new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    if (!new_var)
    {
        perror("malloc");
        return;
    }
    
    ft_strcpy(new_var, name);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);
    
    if ((*env)[i] == NULL)
    {
        char **new_env = realloc(*env, sizeof(char*) * (i + 2));
        if (!new_env)
        {
            free(new_var);
            perror("realloc");
            return;
        }
        *env = new_env;
        (*env)[i + 1] = NULL;
    }
    
    (*env)[i] = new_var;
}
	
void	ft_unset(t_command *cmd, char ***env)
{
	char	*key;
	
	key = cmd->argv[1];
	if (!key)
	{
		write(2, "unset: not enough arguments\n", 28);
		return;
	}
	
	int	i;
	int	j;
	int	len;
	
	i = 0;
	len = ft_strlen(key);
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], key, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			j = i;
			while((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	ft_env(char **env)
{
	int	i;
	
	i = 0;
	if (env == NULL)
	{
		write(2, "env: no environment variables\n", 31);
		return ;
	}
	while(env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	ft_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1] == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	if (!ft_isdigit_str(args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	if(args[2] != NULL)
	{
		write(1, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	status = ft_atoi(args[1]);
	printf("exit\n");
	exit(status);
}

void	execute_internal_commands(t_command *cmd, char ***env)
{
	int	original_stdin = dup(STDIN_FILENO);
	int	original_stdout = dup(STDOUT_FILENO);

	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL) 
	{
		write(2, "minishell: command not found\n", 29);
		return;
	}
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
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(cmd->argv[1], env);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ft_export(cmd, env);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(cmd, env);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ft_env(*env);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(cmd->argv);
	else
		write(2, "minishell: command not found\n", 29);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

char *get_env_value(char **env, const char *name)
{
	int	i;
	size_t	len;

	i = 0;
	if (!env || !name)
		return NULL;
	len = ft_strlen(name);
	while ( env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return &env[i][len + 1];
		i++;
	}
	return (NULL);
}

