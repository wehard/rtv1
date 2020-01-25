/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 19:44:17 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>


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
		hit->t2 = (-b + sqrt(disc)) / (2.0f * a);
		if (hit->t > MAX_DISTANCE || hit->t < MIN_DISTANCE)
			return (FALSE);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = calc_cylinder_normal(cyl, hit);
		return (TRUE);
	}
	return (FALSE);
}
