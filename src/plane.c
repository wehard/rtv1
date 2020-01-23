/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 11:11:04 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/23 12:56:44 by wkorande         ###   ########.fr       */
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
/*
int		intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit)
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
	{
		hit->point = point_on_ray(ray, hit->t);
		return (1);
	}
	return (0);
}
*/

// k = z, t = (k - az) / bz;
static int		intersects_xy(t_ray *ray, t_object *plane, double k, t_raycasthit *hit)
{
	double t;
	double x;
	double y;

	t = (k - ray->origin.z) / ray->direction.z;
	if (t < MIN_DISTANCE || t > MAX_DISTANCE)
		return (FALSE);
	x = ray->origin.x + t * ray->direction.x;
	y = ray->origin.y + t * ray->direction.y;
	if (x < -plane->scale.x || x > plane->scale.x || y < -plane->scale.y || y > plane->scale.y)
		return (FALSE);
	hit->t = t;
	hit->normal = ft_make_vec3(0, 0, 1);
	return (TRUE);
}

static int		intersects_xz(t_ray *ray, t_object *plane, double k, t_raycasthit *hit)
{
	double t;
	double x;
	double z;

	t = (k - ray->origin.y) / ray->direction.y;
	if (t < MIN_DISTANCE || t > MAX_DISTANCE)
		return (FALSE);
	x = ray->origin.x + t * ray->direction.x;
	z = ray->origin.z + t * ray->direction.z;
	if (x < -plane->scale.x || x > plane->scale.x || z < -plane->scale.z || z > plane->scale.z)
		return (FALSE);
	hit->t = t;
	hit->normal = ft_make_vec3(0, 1, 0);
	return (TRUE);
}

static int		intersects_yz(t_ray *ray, t_object *plane, double k, t_raycasthit *hit)
{
	double t;
	double y;
	double z;

	t = (k - ray->origin.x) / ray->direction.x;
	if (t < MIN_DISTANCE || t > MAX_DISTANCE)
		return (FALSE);
	y = ray->origin.y + t * ray->direction.y;
	z = ray->origin.z + t * ray->direction.z;
	if (y < -plane->scale.y || y > plane->scale.y || z < -plane->scale.z || z > plane->scale.z)
		return (FALSE);
	hit->t = t;
	hit->normal = ft_make_vec3(1, 0, 0);
	return (TRUE);
}

int		intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit)
{
	if (plane->normal.z > 0)
		return (intersects_xy(ray, plane, plane->position.z, hit));
	if (plane->normal.y > 0)
		return (intersects_xz(ray, plane, plane->position.y, hit));
	if (plane->normal.x > 0)
		return (intersects_yz(ray, plane, plane->position.x, hit));
	return (FALSE);
}
