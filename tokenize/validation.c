#include "../include/minishell.h"

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

bool	is_metacharacter(char c)
{
	return (strchr("|&;()<>\t\n", c));
}

bool	is_ctrlop(char	*str)
{
	char	*op[] = {"||", "&&", "(", ")", ";;", ";&", "&", ";", "|&", "\n", ">>", "<<"};
	int		i;
	int		length;

	length = sizeof(op) / sizeof(op[0]);
	i = -1;
	while (++i < length)
	{
		if (strncmp(str, op[i], strlen(op[i])) == 0)
			return (true);
	}
	return (false);
}

bool	is_word(char *str)
{
	return (!is_ctrlop(str) && !is_metacharacter(*str) && !is_blank(*str));
}
