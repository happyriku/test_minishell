#include "../include/minishell.h"

void	cleanup_token(t_token *token)
{
	t_token	*cur;
	t_token *next;

	cur = token;
	while (cur)
	{
		printf("cur->word : %s\n", cur->word);
		cur = cur->next;
	}
	while (token)
	{
		if (token->word)
			free(token->word);
		if (!token->next)
		{
			free(token);
			break ;
		}
		next = token->next;
		free(token);
		token = next;
	}
	token = NULL;
}
