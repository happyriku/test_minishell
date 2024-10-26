#include "../include/minishell.h"

int	get_std_fd(char *word)
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

t_node	*redirect_node(t_token **rest, char *word, t_node_kind kind)
{
	t_node	*redirect;

	redirect = malloc(sizeof(t_node));
	if (!redirect)
		return (NULL);
	redirect->filename = strdup(word);
	redirect->delimiter = NULL;
	redirect->next = NULL;
	redirect->kind = kind;
	redirect->std_fd = get_std_fd((*rest)->word);
	*rest = (*rest)->next;
	return (redirect);
}

t_node	*heredoc_node(t_token **rest, char *word, t_node_kind kind)
{
	t_node	*redirect;

	redirect = malloc(sizeof(t_node));
	if (!redirect)
		return (NULL);
	redirect->delimiter = remove_quote(word);
	redirect->filename = NULL;
	redirect->next = NULL;
	redirect->kind = kind;
	redirect->std_fd = get_std_fd((*rest)->word);
	if (pipe(redirect->pipefd) == -1)
		fatal_error("pipe");
	*rest = (*rest)->next;
	return (redirect);
}

bool	has_redirect(t_token *token)
{
	if (strcmp(token->word, ">") == 0 || strcmp(token->word, "<") == 0
		|| strcmp(token->word, ">>") == 0)
		return (true);
	else
		return (false);
}
