/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_vec3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:15:48 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 18:16:27 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "libft.h"

t_vec3	ft_parse_vec3(char *str)
{
	t_vec3	v;
	char	*token;

	str = ft_strstr(str, " ");
	token = ft_strtok(str, " ");
	v.x = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.y = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.z = ft_strtod(token);
	return (v);
}
