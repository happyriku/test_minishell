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

void	cleanup_node(t_node *node)
{
	t_node	*cur;
	t_node	*next;
	int		i;
	t_token	*args;
	t_token	*tmp;

	//cur = *node;
	while (node)
	{
		if (node->args)
		{
			i = -1;
			while (node->args->arr && node->args->arr[++i])
				free(node->args->arr[i]);
			if (node->args->arr)
				free(node->args->arr);
			if (node->args->word)
				free(node->args->word);
			if (node->args->next)
			{
				tmp = node->args->next;
				free(node->args);
				node->args = tmp;
			}
			else
				node->args = NULL;
		}
		cleanup_redirect(node->redirect);
		cur = node->next;
		free(node);
		node = cur;
	}
	//cleanup_node_args((*node)->args);
	//cleanup_redirect((*node)->redirect);
	// while (*node)
	// {
	// 	cur = (*node)->next;
	// 	free(*node);
	// 	*node = cur;
	// }
}
