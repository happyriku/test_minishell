#include "include/minishell.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char		*dest;
	int			size;
	size_t		i;

	if (!s1)
		return (NULL);
	size = strlen(s1) + 1;
	dest = (char *)malloc(size * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = -1;
	while (s1[++i] && i < n)
		dest[i] = s1[i];
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	size_t	s1_len;
	size_t	s2_len;
	char	*str;

	s1_len = strlen(s1);
	s2_len = strlen(s2);
	str = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s1);
	return (str);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	size_src;
	size_t	size_dst;
	size_t	j;

	size_src = strlen(src);
	size_dst = strlen(dst);
	if (dstsize < size_dst + 1)
		return (dstsize + size_src);
	i = size_dst;
	j = 0;
	while (i < dstsize - 1 && src[j])
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (size_dst + size_src);
}

char	*ft_strncpy(char *dst, char *src, int n)
{
	int	i;

	if (!src)
		return (NULL);
	i = 0;
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	ft_lstsize(t_token *lst)
{
	int size;

    if (!lst)
        return (0);
	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}
