/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 09:26:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 17:49:58 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"

int	intersects_sphere(t_ray *ray, t_object *sphere, t_hit *hit)
{
	t_quadratic	q;
	t_vec3		oc;

	oc = ft_sub_vec3(ray->origin, sphere->position);
	q.a = ft_dot_vec3(ray->direction, ray->direction);
	q.b = 2.0 * ft_dot_vec3(oc, ray->direction);
	q.c = ft_dot_vec3(oc, oc) - SQR(sphere->radius);
	if (solve_quadratic(q, &hit->t, &hit->t2))
	{
		if (hit->t > hit->t2)
			ft_swap_d(&hit->t, &hit->t2);
		if (hit->t < 0)
		{
			hit->t = hit->t2;
			if (hit->t < 0)
				return (FALSE);
		}
		if (hit->t > MAX_DISTANCE)
			return (FALSE);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point,
			sphere->position));
		return (TRUE);
	}
	return (FALSE);
}
