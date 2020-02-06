/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_rgba.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:17:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 14:43:59 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "libft.h"

t_rgba	ft_parse_rgba(char *line)
{
	t_rgba	c;
	char	*token;

	c = ft_make_rgba(0, 0, 0, 1);
	if (!line)
		return (c);
	line = ft_strstr(line, " ");
	token = ft_strtok(line, " ");
	if (token)
		c.r = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	if (token)
		c.g = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	if (token)
		c.b = ft_strtod(token);
	c.a = 1.0;
	return (c);
}
