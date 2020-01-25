/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 19:31:45 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <math.h>
#include "libft.h"
#include "ft_printf.h"

t_object	make_plane()
{
	t_object t;
	t.type = PLANE;
	t.position = ft_make_vec3(0.0, 0.0, 0.0);
	t.radius = 1.0;
	t.rotation = ft_make_vec3(0.0, 0.0, 0.0);
	t.scale = ft_make_vec3(1.0, 1.0, 1.0);
	t.color = ft_make_rgba(1.0, 1.0, 1.0, 1.0);
	t.normal = ft_make_vec3(0.0, 1.0, 0.0);
	return  (t);
}

int		intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit)
{
	hit->t = ft_dot_vec3(ft_sub_vec3(plane->position, ray->origin), plane->normal) / ft_dot_vec3(ray->direction, plane->normal);
	hit->point = point_on_ray(ray, hit->t);
	if (plane->scale.x > 0 && plane->scale.y > 0 && plane->scale.z > 0)
	{
		if (hit->point.x > plane->position.x + (plane->scale.x / 2.0) || hit->point.x < plane->position.x - (plane->scale.x / 2.0))
			return (FALSE);
		if (hit->point.z > plane->position.z + (plane->scale.z / 2.0) || hit->point.z < plane->position.z - (plane->scale.z / 2.0))
			return (FALSE);
		if (hit->point.y > plane->position.y + (plane->scale.y / 2.0) || hit->point.y < plane->position.y - (plane->scale.y / 2.0))
			return (FALSE);
	}
	if (hit->t > 0)
	{
		hit->object = plane;
		hit->normal = ft_normalize_vec3(hit->object->normal);
		return (TRUE);
	}
	return (FALSE);
}
