#include "../include/minishell.h"

void	signal_handling(int signum)
{
	if (signum == SIGINT)
	{
		g_info.signal = SIGINT;
		rl_done = 1;
	}
	return ;
}

int	check_state(void)
{
	if (g_info.signal == SIGINT)
	{
		rl_replace_line("^C", 0);
		g_info.signal = 0;
	}
	return (0);
}

void	setup_signal(void)
{
	struct sigaction	sa;

	sa.sa_flags = 0;
	rl_done = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handling;
	rl_event_hook = check_state;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		fatal_error("sigaction");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		fatal_error("sigaction");
}