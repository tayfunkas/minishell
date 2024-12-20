/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:27:58 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 06:43:24 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	parent_sigint_handler(int sig)
{
	g_signal = sig;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_sigint_handler(int sig)
{
	g_signal = sig;
	write(STDERR_FILENO, "\n", 1);
}

void	child_sigquit_handler(int sig)
{
	g_signal = sig;
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

void	setup_signal(void)
{
	signal(SIGINT, parent_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

	//struct termios	term;
	//if (tcgetattr(0, &term) != 0)
	//	perror("tcgetattr");
	//term.c_lflag &= ~ECHOCTL;
	//if (tcsetattr(0, 0, &term) != 0)
	//	perror("tcsetattr");