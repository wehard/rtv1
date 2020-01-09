/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/09 18:58:44 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"
#include <stdio.h>

int	main(int argc, char**argv)
{
	t_vec3 v1, v2;

	v1 = make_vec3(1.0, 0.0, 0.0);
	v1 = mul_vec3(v1, 4.0);
	v1 = div_vec3(v1, 2.0);
	ft_printf("len: %.16f\n", len_vec3(v1));
	ft_printf("len normalized: %.16f\n", len_vec3(normalize_vec3(v1)));

	return (0);
}
