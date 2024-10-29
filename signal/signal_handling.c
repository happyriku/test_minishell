#include "../include/minishell.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_info.signal = SIGINT;
		rl_done = 1;
	}
	return ;
}

void	handle_sigint(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = signal_handler;
	if (sigaction(signum, &sa, NULL) == -1)
		fatal_error("sigaction");
}

void	handler_sigquit(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) == -1)
		fatal_error("sigaction");
}

void	reset_signal(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) == -1)
		fatal_error("sigaction");
}
