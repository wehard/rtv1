/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/27 19:27:15 by wkorande         ###   ########.fr       */
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
	rot.x = ft_deg_to_rad(rot.x);
	rot.y = ft_deg_to_rad(rot.y);
	rot.z = ft_deg_to_rad(rot.z);

	res.x = v.x;
	res.y = v.y * cos(rot.x) + v.z * sin(rot.x);
	res.z = -v.y * sin(rot.x) + v.z * cos(rot.x);
	v = (t_vec3) {res.x, res.y, res.z};
	res.x = v.x * cos(rot.y) + v.z * sin(rot.y);
	res.y = v.y;
	res.z = -v.x * sin(rot.y) + v.z * cos(rot.y);
	v = (t_vec3) {res.x, res.y, res.z};
	res.x = v.x * cos(rot.z) - v.y * sin(rot.z);
	res.y = v.x * sin(rot.z) + v.y * cos(rot.z);
	res.z = v.z;

	c->end = ft_add_vec3(c->position, res);
	c->start = c->position;

	//ft_printf("end  : %.3f, %.3f, %.3f\n", c->end.x, c->end.y, c->end.z);
	//ft_printf("start: %.3f, %.3f, %.3f\n", c->start.x, c->start.y, c->start.z);
}


int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	double t1, t2;
	t_quadratic q;
	double	disc;

	t_vec3 pdp, eyexpdp, rdxpdp;
	pdp = ft_sub_vec3(cyl->start, cyl->end);
	eyexpdp = ft_cross_vec3(ft_sub_vec3(ray->origin, cyl->end), pdp);
	rdxpdp = ft_cross_vec3(ray->direction, pdp);
	q.a = ft_dot_vec3(rdxpdp, rdxpdp);
	q.b = 2 * ft_dot_vec3(rdxpdp, eyexpdp);
	q.c = ft_dot_vec3(eyexpdp, eyexpdp) - (cyl->radius * cyl->radius * ft_dot_vec3(pdp, pdp));
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
