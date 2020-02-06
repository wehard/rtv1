/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 19:00:58 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 18:08:25 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

/*
** O ray origin
** D ray direction
** C cone tip
** V cone direction
** theta half cone angle
*/

static t_vec3	calc_cone_normal(t_object *cone, t_hit *hit)
{
	t_vec3 n;
	t_vec3 cp;
	t_vec3 cone_dir;

	cone_dir = ft_normalize_vec3(ft_sub_vec3(cone->end, cone->start));
	cp = ft_sub_vec3(hit->point, cone->position);
	n = ft_sub_vec3(cp, ft_mul_vec3(cone_dir,
		(ft_len_vec3(cp) / cos(cone->radius))));
	n = ft_normalize_vec3(n);
	return (n);
}

void			rotate_cone(t_object *c, t_vec3 rot)
{
	t_vec3 res;
	t_vec3 v;

	v = ft_make_vec3(0, -1, 0);
	res = ft_rotate_vec3(v, rot);
	c->end = ft_add_vec3(c->position, res);
	c->start = c->position;
}

static int		test_truncated(t_object *cone, t_hit *hit)
{
	if (cone->scale.y != 0 && ft_len_vec3(ft_sub_vec3(hit->point,
		cone->position)) > cone->scale.y)
		return (FALSE);
	return (TRUE);
}

int				intersects_cone(t_ray *ray, t_object *cone, t_hit *hit)
{
	t_quadratic	q;
	t_vec3		x;
	t_vec3		cone_dir;

	cone_dir = ft_normalize_vec3(ft_sub_vec3(cone->end, cone->start));
	x = ft_sub_vec3(ray->origin, cone->position);
	q.a = ft_dot_vec3(ray->direction, cone_dir);
	q.a = ft_dot_vec3(ray->direction, ray->direction) -
		(1 + cone->radius * cone->radius) * q.a * q.a;
	q.b = 2.0 * (ft_dot_vec3(ray->direction, x) -
		(1 + cone->radius * cone->radius)
		* ft_dot_vec3(ray->direction, cone_dir) * ft_dot_vec3(x, cone_dir));
	q.c = ft_dot_vec3(x, cone_dir);
	q.c = ft_dot_vec3(x, x) - (1 + cone->radius * cone->radius) * q.c * q.c;
	if (solve_quadratic(q, &hit->t, &hit->t2) && (hit->t > 0) &&
		(hit->t < MAX_DISTANCE))
	{
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = calc_cone_normal(cone, hit);
		if ((ft_dot_vec3(
				ft_sub_vec3(hit->point, cone->position), cone_dir) < 0))
			return (FALSE);
		return (test_truncated(cone, hit));
	}
	return (FALSE);
}
