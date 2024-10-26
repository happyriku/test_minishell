#include "../include/minishell.h"

void	syntax_error(void)
{
	g_info.syntax_error = true;
	printf("syntax error\n");
}

void	expand(t_node *node)
{
	t_token	*args;

	args = node->args;
	expand_variable(args);
}
