#include "../include/minishell.h"

bool	is_alpha_under(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_variable(char *word)
{
	return (*word == '$' && is_alpha_under(*(word + 1)));
}

bool	is_special_parameter(char *word)
{
	return (*word == '$' && *(word + 1) == '?');
}