#include "../include/minishell.h"

t_node	*new_node(char *word, t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = kind;
	node->next = NULL;
	node->redirect = NULL;
	return (node);
}

int	filter_redirect(char *word)
{

	if (strncmp(word, ">>", 2) == 0)
		return (STDOUT_FILENO);
	else if (strncmp(word, "<<", 2) == 0)
		return (STDIN_FILENO);
	else if (strcmp(word, ">") == 0)
		return (STDOUT_FILENO);
	else if (strcmp(word, "<") == 0)
		return (STDIN_FILENO);
}

t_node	*new_redirect_node(t_token **rest, char *word, t_node_kind kind)
{
	t_node	*redirect;

	redirect = malloc(sizeof(t_node));
	if (!redirect)
		return (NULL);
	redirect->filename = word;
	redirect->next = NULL;
	redirect->kind = kind;
	redirect->std_fd = filter_redirect((*rest)->word);
	*rest = (*rest)->next;
	return (redirect);
}

t_node	*new_heredoc_node(t_token **rest, char *word, t_node_kind kind)
{
	t_node	*redirect;

	redirect = malloc(sizeof(t_node));
	if (!redirect)
		return (NULL);
	redirect->delimiter = word;
	redirect->next = NULL;
	redirect->kind = kind;
	redirect->std_fd = filter_redirect((*rest)->word);
	if (pipe(redirect->pipefd) == -1)
		perror("pipe");
	*rest = (*rest)->next;
	return (redirect);
}

bool	has_redirect(t_token *token)
{
	if (strcmp(token->word, ">") == 0 || strcmp(token->word, "<") == 0
		|| strcmp(token->word, ">>") == 0 || strcmp(token->word, "<<") == 0)
		return (true);
	else
		return (false);
}

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
		cur = cur->next;
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

int	init_node(t_node **node)
{
	*node = new_node(NULL, ND_SIMPLE_CMD);
	if (!(*node))
		return (0);
	(*node)->args = new_token(NULL, TK_EOF);
	if (!(*node)->args)
		return (0);
	(*node)->args = (*node)->args->next;
	return (1);
}

t_node	*get_node(t_token *token)
{
	t_node	*node;
	t_token	*args;
	t_node	*redirect;

	if (!init_node(&node))
		return (NULL);
	while (token)
	{
		if (token->kind == TK_EOF)
			break ;
		else if (token->kind == TK_WORD)
			append_token(&(node->args), new_token(token->word, TK_WORD));
		else if (strncmp(token->word, ">>", 2) == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_APPEND));
		else if (strncmp(token->word, "<<", 2) == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), new_heredoc_node(&token, token->next->word, ND_HEREDOC));
		else if (strcmp(token->word, ">") == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_OUT));
		else if (strcmp(token->word, "<") == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_IN));
		token = token->next;
	}
	append_token(&(node->args), new_token(NULL, TK_EOF));
	return (node);	
}

t_node	*parse(t_token *token)
{
	t_node	*node;
	t_token	*args;

	node = get_node(token);
	if (ft_lstsize(node->args) == 1)
	{
		g_info.syntax_error = true;
		printf("zsh: parse error\n");
	}
	return (node);
}
