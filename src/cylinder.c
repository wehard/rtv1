/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/24 17:47:45 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	t_vec3 x;
	double a;
	double b;
	double c;
	double disc;

	x = ft_sub_vec3(ray->origin, cyl->position);
	a = ft_dot_vec3(ray->direction, cyl->normal); //!
	a = ft_dot_vec3(ray->direction, ray->direction) - a * a;
	b = 2 * (ft_dot_vec3(ray->direction, x) - ft_dot_vec3(ray->direction, cyl->normal)
		* ft_dot_vec3(x, cyl->normal));
	c = ft_dot_vec3(x, cyl->normal);
	c = ft_dot_vec3(x, x) - c * c - cyl->radius * cyl->radius;
	disc = b * b - 4 * a * c;
	if (disc < 0)
		return (FALSE);
	else
	{
		hit->t = (-b - sqrt(disc)) / (2.0*a);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, cyl->position));
	}
	return (TRUE);
}
