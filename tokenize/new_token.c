#include "../include/minishell.h"

t_token	*new_token(char *str, t_kind kind)
{
	t_token	*new_token;

	// if (str == MALLOC_ERROR)
	// 	return (NULL);
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (kind == TK_EOF)
		new_token->word = NULL;
	else
		new_token->word = str;
	new_token->next = NULL;
	new_token->kind = kind;
	return (new_token);
}

t_token	*tokdup(t_token *token)
{
	t_token *new_tok;

	new_tok = (t_token *)malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	new_tok->word = strdup(token->word);
	if (!new_tok->word)
		fatal_error("strdup");
	new_tok->kind = token->kind;
	return (new_tok);

}