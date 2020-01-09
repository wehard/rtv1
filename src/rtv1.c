/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/09 23:07:47 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"
#include <stdio.h>
#include "ray.h"

int	main(void)
{

	t_vec3 v1;
	t_ray ray;

	v1 = make_vec3(1.0, 0.0, 0.0);
	v1 = mul_vec3(v1, 4.0);
	v1 = div_vec3(v1, 2.0);
	ft_printf("%-15s %f\n", "len:", len_vec3(v1));
	ft_printf("%-15s %f\n", "len normalized:", len_vec3(normalize_vec3(v1)));

	ray.origin = make_vec3(0.0, 0.0, 1.0);
	ray.direction = make_vec3(0.0, 0.0, 1.0);

	ft_printf("%-15s %f\n", "ray length:", len_vec3(add_vec3(ray.origin, ray.direction)));

	ft_printf("%-15s %f\n", "len ray point:", len_vec3(add_vec3(ray.origin, point_on_ray(&ray, 1430.0))));
	return (0);
}
