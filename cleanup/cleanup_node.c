#include "../include/minishell.h"

void	cleanup_node_args(t_token *args)
{
	t_token *cur;

	if (!(args))
		return ;
	while (args)
	{
		if (!(args)->next)
		{
			free(args);
			return ;
		}
		cur = (args)->next;
		free(args);
		args = cur;
	}
}

void	cleanup_node(t_node **node)
{
	t_node	*cur;
	t_node	*next;

	cleanup_node_args((*node)->args);
	while (*node)
	{
		cur = (*node)->next;
		free(*node);
		*node = cur;
	}
}
