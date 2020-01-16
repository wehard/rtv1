/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 12:09:33 by wkorande         ###   ########.fr       */
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
	t.position = make_vec3(0.0, 0.0, 0.0);
	t.radius = 1.0;
	t.rotation = make_vec3(0.0, 0.0, 0.0);
	t.scale = make_vec3(1.0, 1.0, 1.0);
	t.color = ft_make_rgba(1.0, 1.0, 1.0, 1.0);
	t.normal = make_vec3(0.0, 1.0, 0.0);
	return  (t);
}

int		intersects_plane(t_ray *ray, t_shape *plane, t_raycasthit *hit)
{
	hit->t = dot_vec3(sub_vec3(plane->position, ray->origin), plane->normal) / dot_vec3(ray->direction, plane->normal);
	if (hit->t > 0)
		return (1);
	return (0);
}
