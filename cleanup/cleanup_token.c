#include "../include/minishell.h"

void	cleanup_token(t_token **token)
{
	t_token	*cur;
	t_token *next;

	cur = *token;
	while (cur)
	{
		next = cur->next;
		if (cur->word)
			free(cur->word);
		free(cur);
		cur = next;
	}
	*token = NULL;
}
