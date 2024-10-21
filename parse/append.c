#include "../include/minishell.h"

void	append_token(t_token **args, t_token *token)
{
	t_token *cur;

	if (!token)
		return ;
	if (!(*args))
	{
		*args = token;
		return ;
	}
	cur = *args;
	while (cur->next)
	{
		cur = cur->next;
	}
	cur->next = token;
}

void	append_node(t_node	**redirect, t_node *node)
{
	t_node	*cur;

	if (!node)
		return ;
	if (!(*redirect))
	{
		*redirect = node;
		return ;
	}
	cur = *redirect;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
	return ;
}
