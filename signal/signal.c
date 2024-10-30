#include "../include/minishell.h"

int	check_state(void)
{
	if (g_info.signal == SIGINT)
	{
		// if (g_info.last_status >= 128)
		//  	rl_replace_line("", 0); //readlineの入力バッファを空にしてプロンプトを再表示
		rl_done = 1; //現在の入力処理を適切に終了し，新しいプロンプトの表示が可能
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
	if (isatty(STDIN_FILENO))
		rl_event_hook = check_state;
	handle_sigint(SIGINT);
	handler_sigquit(SIGQUIT);
}