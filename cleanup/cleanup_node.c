#include "../include/minishell.h"

void	cleanup_node_args(t_token *args)
{
	t_token	*cur;

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

void	cleanup_redirect(t_node *redirect)
{
	t_node	*cur;

	if (!redirect)
		return ;
	while (redirect)
	{
		if (!redirect->next)
			return ;
		cur = redirect->next;
		free(redirect);
		redirect = cur;
	}
}

void	cleanup_node(t_node **node)
{
	t_node	*cur;
	t_node	*next;

	cleanup_node_args((*node)->args);
	cleanup_redirect((*node)->redirect);
	while (*node)
	{
		cur = (*node)->next;
		free(*node);
		*node = cur;
	}
}
