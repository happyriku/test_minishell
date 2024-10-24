#include "include/minishell.h"

extern char	**environ;
t_info g_info = {};

int	interpret(char *input)
{
	int		status;
	char	*path;
	int		i;
	t_token	*token;
	t_token	*tmp;
	t_info	info;
	t_node	*node;
	int		res;

	if (*input < 1)
		return (0);
	token = tokenize(input);
	if (!token)
		return (1);
	node = parse(token);
	if (g_info.syntax_error)
	{
		//parseの段階ではnode->args->arrが初期化されていないのでclean_nodeでsegvが発生する
		g_info.syntax_error = false;
		return (cleanup_token(token), 0);
	}
	expand(node);
	if (g_info.syntax_error)
	{
		//expandの段階ではnode->args->arrが初期化されていないのでcleanup_nodeでsegvが発生する
		g_info.syntax_error = false;
		return (cleanup_token(token), 0);
	}
	res = exec(node);
	if (res == MALLOC_ERROR)
		return (cleanup_node(node), 1);
	else if (res == -1)
		return (cleanup_node(node), EXIT);
	else if (res == 0)
		return (cleanup_node(node), 0);
	else if (res == 1)
	{
		cleanup_node(node);
		exit(0);
	}
}
