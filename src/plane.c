/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/15 17:51:52 by wkorande         ###   ########.fr       */
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
	t.position = make_vec3(0.0f, 0.0f, 0.0f);
	t.radius = 1.0f;
	t.rotation = make_vec3(0.0f, 0.0f, 0.0f);
	t.scale = make_vec3(1.0f, 1.0f, 1.0f);
	t.color = make_vec3(1.0f, 1.0f, 1.0f);
	t.normal = make_vec3(0.0f, 1.0f, 0.0f);
	return  (t);
}

int		intersects_plane(t_ray *ray, t_shape *plane, t_raycasthit *hit)
{
	hit->t = dot_vec3(sub_vec3(plane->position, ray->origin), plane->normal) / dot_vec3(ray->direction, plane->normal);
	if (hit->t > 0)
		return (1);
	return (0);
}
