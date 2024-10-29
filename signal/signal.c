#include "../include/minishell.h"

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