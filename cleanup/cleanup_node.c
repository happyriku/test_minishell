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
	int		i;
	t_token	*args;
	t_token	*tmp;

	cur = *node;
	while (cur)
	{
		if (cur->args)
		{
			i = -1;
			while (cur->args->arr && cur->args->arr[++i])
				free(cur->args->arr[i]);
			if (cur->args->arr)
				free(cur->args->arr);
			if (cur->args->word)
				free(cur->args->word);
			if (cur->args->next)
			{
				tmp = cur->args->next;
				free(cur->args);
				cur->args = tmp;
			}
			else
				cur->args = NULL;
		}
		cur = cur->next;
	}
	//cleanup_node_args((*node)->args);
	cleanup_redirect((*node)->redirect);
	// while (*node)
	// {
	// 	cur = (*node)->next;
	// 	free(*node);
	// 	*node = cur;
	// }
}
