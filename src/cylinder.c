/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/27 15:18:17 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>
#include "ft_printf.h"


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
	//t_mat4x4 m = ft_create_rot_mat4(rot);
	//t_vec3 r = ft_mul_vec3_mat4(ft_make_vec3(0, 1, 0), m);
	t_vec3 r = ft_make_vec3(0, 1, 0);
	t_vec3 p = ft_make_vec3(0, 0, 0);
	p.x = r.x * cos(ft_deg_to_rad(rot.z)) - r.y * sin(ft_deg_to_rad(rot.z));
	p.y = r.x * sin(ft_deg_to_rad(rot.z)) + r.y * cos(ft_deg_to_rad(rot.z));

	c->end = ft_add_vec3(c->position, p);
	c->start = c->position;

	//ft_printf("end  : %.3f, %.3f, %.3f\n", c->end.x, c->end.y, c->end.z);
	//ft_printf("start: %.3f, %.3f, %.3f\n", c->start.x, c->start.y, c->start.z);
}


int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	double a, b, c;
	double	disc;

	t_vec3 pdp, eyexpdp, rdxpdp;
	pdp = ft_sub_vec3(cyl->start, cyl->end);
	eyexpdp = ft_cross_vec3(ft_sub_vec3(ray->origin, cyl->end), pdp);
	rdxpdp = ft_cross_vec3(ray->direction, pdp);
	a = ft_dot_vec3(rdxpdp, rdxpdp);
	b = 2 * ft_dot_vec3(rdxpdp, eyexpdp);
	c = ft_dot_vec3(eyexpdp, eyexpdp) - (cyl->radius * cyl->radius * ft_dot_vec3(pdp, pdp));
	disc = (b * b) - (4.0f * a * c);
	if (disc < 0)
		return (FALSE);
	else
	{
		hit->t = (-b - sqrt(disc)) / (2.0f * a);
		if (hit->t > MAX_DISTANCE || hit->t < MIN_DISTANCE)
			return (FALSE);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = calc_cylinder_normal(cyl, hit);
		return (TRUE);
	}
	return (FALSE);
}
