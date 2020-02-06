/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_vec3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:15:48 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 15:23:25 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "libft.h"

t_vec3	ft_parse_vec3(char *str)
{
	t_vec3	v;
	char	*token;

	v = ft_make_vec3(0, 0, 0);
	if (!str)
		return (v);
	str = ft_strstr(str, " ");
	token = ft_strtok(str, " ");
	if (token)
		v.x = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	if (token)
		v.y = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	if (token)
		v.z = ft_strtod(token);
	return (v);
}
