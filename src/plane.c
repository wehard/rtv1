/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/31 15:42:49 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <math.h>
#include "libft.h"
#include "ft_printf.h"

void	rotate_plane(t_object *p, t_vec3 rot)
{
	t_vec3 res;
	t_vec3 v = ft_make_vec3(0, 1, 0);

	res = ft_rotate_vec3(v, rot);
	p->normal = ft_normalize_vec3(res);
}

int		intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit)
{
	double t;
	// if (ft_dot_vec3(ray->direction, plane->position) < 0)
	// 	return (FALSE);
	double d;
	d = ft_dot_vec3(plane->normal, ray->direction);
	if (ray->origin_object &&  d < 0.001) // if shadow ray
		return (FALSE);
	t = ft_dot_vec3(ft_sub_vec3(plane->position, ray->origin), plane->normal) / d;
	if (t > 0.0001)
	{
		hit->t = t;
		hit->point = point_on_ray(ray, hit->t);
		hit->object = plane;
		hit->normal = plane->normal;
		if (plane->scale.x > 0 && plane->scale.y > 0 && plane->scale.z > 0)
		{
			if (hit->point.x > plane->position.x + (plane->scale.x / 2.0) || hit->point.x < plane->position.x - (plane->scale.x / 2.0))
				return (FALSE);
			if (hit->point.z > plane->position.z + (plane->scale.z / 2.0) || hit->point.z < plane->position.z - (plane->scale.z / 2.0))
				return (FALSE);
			if (hit->point.y > plane->position.y + (plane->scale.y / 2.0) || hit->point.y < plane->position.y - (plane->scale.y / 2.0))
				return (FALSE);
		}
		return (TRUE);
	}
	return (FALSE);
}
