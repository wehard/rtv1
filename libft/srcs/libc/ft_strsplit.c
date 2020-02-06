/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 12:09:06 by wkorande          #+#    #+#             */
/*   Updated: 2019/10/20 17:17:07 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	ft_wordlen(char *str, char end)
{
	int i;

	i = 0;
	while (str[i] != '\0' && str[i] != end)
		++i;
	return (i);
}

char		**ft_strsplit(char const *s, char c)
{
	char	**strs;
	int		nwords;
	int		i;
	int		len;

	nwords = ft_nwords((char*)s, c);
	if (!(strs = (char**)malloc(sizeof(char*) * nwords + 1)))
		return (NULL);
	i = 0;
	while (i < nwords)
	{
		while (*s == c)
			s++;
		len = ft_wordlen((char*)s, c);
		if (!(strs[i] = (char*)malloc(sizeof(char) * len + 1)))
			return (NULL);
		strs[i] = ft_strncpy(strs[i], s, len);
		strs[i][len] = '\0';
		s += len;
		i++;
	}
	strs[i] = 0;
	return (strs);
}
