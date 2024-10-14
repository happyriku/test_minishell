#include "../include/minishell.h"

t_node	*new_node(char *word, t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		fatal_error("malloc");
	node->kind = kind;
	node->next = NULL;
	node->redirect = NULL;
	return (node);
}

void append_argv(t_token **args)
{
    int argc;
    t_token *cur;

    cur = *args;
    argc = 0;
    while (cur && cur->kind != TK_EOF)
    {
        argc++;
        cur = cur->next;
    }
    (*args)->arr = (char **)malloc(sizeof(char *) * (argc + 1));
    if (!(*args)->arr)
        fatal_error("malloc");
    argc = 0;
    cur = *args;
    while (cur && cur->kind != TK_EOF)
    {
        (*args)->arr[argc] = strdup(cur->word);
        if (!((*args)->arr[argc]))
            fatal_error("malloc");
        cur = cur->next;
        argc++;
    }
    (*args)->arr[argc] = NULL; 
}

t_node	*get_node(t_token **rest, t_token *token)
{
	t_node	*node;
	t_token	*args;
	t_node	*redirect;

	node = new_node(NULL, ND_SIMPLE_CMD);
	node->args = new_token(NULL, TK_EOF);
	if (!node->args)
		return (NULL);
	node->args = node->args->next;
	while (token)
	{
		if (token->kind == TK_EOF)
			break ;
		else if (token->kind == TK_WORD)
			append_token(&(node->args), new_token(token->word, TK_WORD));
		else if (strncmp(token->word, ">>", 2) == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_APPEND));
		else if (strncmp(token->word, "<<", 2) == 0 && token->next->kind == TK_WORD)
		{
			if (token->next->next->word == NULL)
				append_node(&(node->redirect), heredoc_node(&token, token->next->word, ND_HEREDOC));
			else
				token = token->next;
		}
		else if (strcmp(token->word, ">") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_OUT));
		else if (strcmp(token->word, "<") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_IN));
		else if (strcmp(token->word, "|") == 0)
			break ;
		token = token->next;
	}
	append_token(&(node->args), new_token(NULL, TK_EOF));
	append_argv(&(node->args));
	*rest = token;
	return (node);
}

t_node	*parse(t_token *token)
{
	t_node	*node;
	t_token	*args;

	node = get_node(&token, token);
	if (ft_lstsize(node->args) == 1)
	{
		g_info.syntax_error = true;
		printf("zsh: parse error\n");
	}
	if (token->word && strcmp(token->word, "|") == 0)
		node->next = parse(token->next);
	return (node);
}
