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
		rl_replace_line("", 0); //readlineの入力バッファを空にしてプロンプトを再表示
		g_info.signal = 0;
	}
	else if (g_info.signal = SIGQUIT)
	{
		g_info.signal = 0;
	}
	return (0);
}

void	handle_sigint(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = signal_handling;
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

void	reset_signals(void)
{
	reset_signal(SIGINT);
	reset_signal(SIGQUIT);
}

void	setup_signal(void)
{
	struct sigaction	sa;

	rl_done = 0;
	rl_event_hook = check_state;
	handle_sigint(SIGINT);
	handler_sigquit(SIGQUIT);
}