/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 19:00:58 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/27 18:31:21 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

// O ray origin
// D ray direction
// C cone tip
// V cone direction
// theta half cone angle

static t_vec3 calc_cone_normal(t_object *cone, t_raycasthit *hit)
{
	t_vec3 n;
	t_vec3 cp;
	t_vec3 cone_dir = cone->normal;
	//double angle = 60.0;
	cp = ft_sub_vec3(hit->point, cone->position);
	n = ft_sub_vec3(cp, ft_mul_vec3(cone_dir, (ft_len_vec3(cp) / cos(cone->radius))));
	n = ft_normalize_vec3(n);
	return (n);
}

int		intersects_cone(t_ray *ray, t_object *cone, t_raycasthit *hit)
{
	t_quadratic q;
	double t1, t2;
	double angle = cone->radius;
	t_vec3 cone_dir = cone->normal;
	angle = ft_deg_to_rad(angle/2);

	double ddotv = ft_dot_vec3(ray->direction, cone_dir);
	t_vec3 co = ft_sub_vec3(ray->origin, cone->position);
	double codotv = ft_dot_vec3(co, cone_dir);

	q.a = SQR(ddotv) - SQR(cos(angle));
	q.b = 2.0 * (ddotv * codotv - ft_dot_vec3(ray->direction, co) * SQR(cos(angle)));
	q.c = SQR(codotv) - ft_dot_vec3(co, co) * SQR(cos(angle));
	if (solve_quadratic(q, &t1, &t2))
	{
		hit->t = t1;
		if (hit->t < 0 || (t2 > 0 && t2 < hit->t))
			hit->t = t2;
		if (hit->t < 0)
			return (FALSE);
		hit->point = point_on_ray(ray, hit->t);
		if (ft_len_vec3(ft_sub_vec3(hit->point, cone->position)) > cone->scale.y)
			return (FALSE);
		if (ft_dot_vec3(ft_sub_vec3(hit->point, cone->position), cone_dir) > 0)
			return (FALSE);
		hit->normal = calc_cone_normal(cone, hit);
		return (TRUE);
	}
	return (FALSE);
}
