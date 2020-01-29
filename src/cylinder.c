/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/29 17:19:45 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>
#include "ft_printf.h"
#include "matrix.h"

static t_vec3 calc_cylinder_normal(t_object *c, t_raycasthit *hit)
{
	t_vec3 dir;
	t_vec3 c_to_hit;
	t_vec3 v;
	double d;

	dir = ft_normalize_vec3(ft_sub_vec3(c->end, c->start));
	c_to_hit = ft_sub_vec3(hit->point, c->start);
	d = ft_dot_vec3(dir, c_to_hit);
	v = ft_add_vec3(c->start, ft_mul_vec3(dir, d));
	return (ft_normalize_vec3(ft_sub_vec3(hit->point, v)));
}

void	rotate_cylinder(t_object *c, t_vec3 rot)
{
	t_vec3 res;
	t_vec3 v = ft_make_vec3(0, 1, 0);

	res = ft_rotate_vec3(v, rot);
	c->end = ft_add_vec3(c->position, res);
	c->start = c->position;
}

int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	double t1, t2;
	t_quadratic q;
	double	disc;
	double d;
	t_vec3 pdp, eyexpdp, rdxpdp;
	pdp = ft_sub_vec3(cyl->start, cyl->end);

	//if (ray->origin_object && ft_dot_vec3(ray->direction, ft_sub_vec3(cyl->end, cyl->start)) < 0) // shadow ray
	//	return (FALSE);

	eyexpdp = ft_cross_vec3(ft_sub_vec3(ray->origin, cyl->end), pdp);
	rdxpdp = ft_cross_vec3(ray->direction, pdp);
	q.a = ft_dot_vec3(rdxpdp, rdxpdp);
	q.b = 2 * ft_dot_vec3(rdxpdp, eyexpdp);
	q.c = ft_dot_vec3(eyexpdp, eyexpdp) - (SQR(cyl->radius) * ft_dot_vec3(pdp, pdp));
	if (solve_quadratic(q, &t1, &t2))
	{
		hit->t = t1;
		if (hit->t < 0 || (t2 > 0 && t2 < hit->t))
			hit->t = t2;
		if (hit->t < 0)
			return (FALSE);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = calc_cylinder_normal(cyl, hit);
		return (TRUE);
	}
	return (FALSE);
}
