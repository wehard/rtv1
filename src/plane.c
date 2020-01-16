/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 17:07:30 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <math.h>
#include "libft.h"
#include "ft_printf.h"

t_shape	make_plane()
{
	t_shape t;
	t.type = PLANE;
	t.position = ft_make_vec3(0.0, 0.0, 0.0);
	t.radius = 1.0;
	t.rotation = ft_make_vec3(0.0, 0.0, 0.0);
	t.scale = ft_make_vec3(1.0, 1.0, 1.0);
	t.color = ft_make_rgba(1.0, 1.0, 1.0, 1.0);
	t.normal = ft_make_vec3(0.0, 1.0, 0.0);
	return  (t);
}

int		intersects_plane(t_ray *ray, t_shape *plane, t_raycasthit *hit)
{
	hit->t = ft_dot_vec3(ft_sub_vec3(plane->position, ray->origin), plane->normal) / ft_dot_vec3(ray->direction, plane->normal);
	if (plane->scale.x > 0 && plane->scale.y > 0 && plane->scale.z > 0)
	{
		t_vec3 p = point_on_ray(ray, hit->t);
		if (p.x > plane->position.x + (plane->scale.x / 2.0) || p.x < plane->position.x - (plane->scale.x / 2.0))
			return (0);
		if (p.z > plane->position.z + (plane->scale.z / 2.0) || p.z < plane->position.z - (plane->scale.z / 2.0))
			return (0);
		if (p.y > plane->position.y + (plane->scale.y / 2.0) || p.y < plane->position.y - (plane->scale.y / 2.0))
			return (0);
	}

	if (hit->t > 0)
		return (1);
	return (0);
}
