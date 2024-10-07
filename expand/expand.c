#include "../include/minishell.h"

void	append_char(char **new_word, char word)
{
	char	*memory;

	size_t	len;

	if (*new_word == NULL)
		len = 2;
	else
		len = strlen(*new_word) + 2;
	memory = (char *)malloc(sizeof(char) * (len));
	if (!memory)
		return ;
	if (*new_word == NULL)
	{
		memory[0] = word;
		memory[1] = '\0';
	}
	else
	{
		ft_strncpy(memory, *new_word, len);
		memory[len - 2] = word;
		memory[len - 1] = '\0';
		free(*new_word);
	}
	*new_word = memory;
}

bool	is_closed_quote(char *word)
{
	int	single_quote_count;
	int	double_quote_count;

	single_quote_count = 0;
	double_quote_count = 0;
	while (*word)
	{
		if (*word == SINGLEQUOTE)
			single_quote_count++;
		if (*word == DOUBLEQUOTE)
			double_quote_count++;
		word++;
	}
	if (single_quote_count % 2 != 0 || double_quote_count % 2 != 0)
		return (false);
	return (true);
}

void	syntax_error(void)
{
	g_info.syntax_error = true;
	printf("syntax error\n");
}

void	quote_removal(t_token *args)
{
	char	**word;
	char	*new_word;

	if (args->next == NULL)
		return ;
	if (!is_closed_quote(args->word))
		syntax_error();
	word = &(args->word);
	new_word = NULL;
	while (**word && !is_metacharacter(**word))
	{
		if (**word != SINGLEQUOTE && **word != DOUBLEQUOTE)
			append_char(&new_word, **word);
		(*word)++;
	}
	args->word = new_word;
	quote_removal(args->next);
}

void	expand(t_node *node)
{
	t_token	*args;

	args = node->args;
	quote_removal(args);
}
