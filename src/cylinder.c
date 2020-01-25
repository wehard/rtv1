/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 18:23:23 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>


static t_vec3 compute_cylinder_normal(t_ray *ray, t_object *c, t_raycasthit *hit)
{
	t_vec3 cyldir;

	cyldir = ft_normalize_vec3(ft_sub_vec3(c->end, c->start));

	t_vec3 hitcstart = ft_sub_vec3(hit->point, c->start);

	double d = ft_dot_vec3(cyldir, hitcstart);

	t_vec3 v = ft_add_vec3(c->start, ft_mul_vec3(cyldir, d));

	return (ft_normalize_vec3(ft_sub_vec3(hit->point, v)));
}


int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	double a, b, c;
	double	disc;
	double	t;

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
		hit->normal = compute_cylinder_normal(ray, cyl, hit);
		return (TRUE);
	}
	return (FALSE);
}

/*
int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	//t_vec3	q;
	double a, b, c;
	double	disc;
	double	t;

	t_vec3 pdp, eyexpdp, rdxpdp;

	pdp = ft_sub_vec3(cyl->rotation, cyl->position);
	eyexpdp = ft_cross_vec3(ft_sub_vec3(ray->origin, cyl->position), pdp);
	rdxpdp = ft_cross_vec3(ray->direction, pdp);

	a = ft_dot_vec3(ray->direction, ray->direction) - (ft_dot_vec3(ray->direction, cyl->rotation) * ft_dot_vec3(ray->direction, cyl->rotation));
	b = ft_dot_vec3(ray->direction, x) - (ft_dot_vec3(ray->direction, cyl->rotation) * ft_dot_vec3(x, cyl->rotation));
	b *= 2;
	c = ft_dot_vec3(x, x) - (ft_dot_vec3(x, cyl->rotation) * ft_dot_vec3(x, cyl->rotation)) - (cyl->radius*cyl->radius);
	disc = (b * b) - (4.0f * a * c);

	if (disc < 0)
		return (FALSE);
	else
	{
		hit->t = (-b - sqrt(disc)) / (2.0f * a);
		return (TRUE);
	}
	return (t);
}
*/
