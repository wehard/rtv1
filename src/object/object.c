/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:41:53 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 21:53:16 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <math.h>
#include "vector.h"

void		init_object(t_object *object)
{
	object->type = 0;
	object->position = ft_make_vec3(0.0, 0.0, 0.0);
	object->rotation = ft_make_vec3(0.0, 0.0, 0.0);
	object->scale = ft_make_vec3(0.0, 0.0, 0.0);
	object->normal = ft_make_vec3(0.0, 0.0, 0.0);
	object->color = ft_make_rgba(0.0, 0.0, 0.0, 1.0);
	object->start = ft_make_vec3(0, 0, 0);
	object->end = ft_make_vec3(1, 1, 1);
	object->radius = 0.0;
}

int			intersects_object(t_ray *ray, t_object *object, t_hit *hit)
{
	int hit_found;

	hit_found = 0;
	hit->distance = 0.0f;
	if (object->type == PLANE)
		hit_found = intersects_plane(ray, object, hit);
	else if (object->type == SPHERE)
		hit_found = intersects_sphere(ray, object, hit);
	else if (object->type == CYLINDER)
		hit_found = intersects_cylinder(ray, object, hit);
	else if (object->type == CONE)
		hit_found = intersects_cone(ray, object, hit);
	if (hit_found)
	{
		hit->object = object;
		hit->point = point_on_ray(ray, hit->t);
		hit->distance = hit->t;
	}
	return (hit_found);
}

int			solve_quadratic(t_quadratic q, double *t1, double *t2)
{
	double d;

	*t1 = -1;
	*t2 = -1;
	d = q.b * q.b - 4 * q.a * q.c;
	if (d == 0)
	{
		*t1 = (-q.b - sqrt(d)) / (2.0 * q.a);
		return (1);
	}
	if (d > 0)
	{
		*t1 = (-q.b - sqrt(d)) / (2.0 * q.a);
		*t2 = (-q.b + sqrt(d)) / (2.0 * q.a);
		return (2);
	}
	return (0);
}
