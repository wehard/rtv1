/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:41:53 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 19:12:58 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "ft_printf.h"
#include <math.h>
#include "vector.h"
#include "libft.h"
#include "ft_get_next_line.h"

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

static void	set_object_property(char *line, t_object *object)
{
	if (ft_strnequ(line, "pos", 3))
		object->position = ft_parse_vec3(line);
	else if (ft_strnequ(line, "rot", 3))
		object->rotation = ft_parse_vec3(line);
	else if (ft_strnequ(line, "sca", 3))
		object->scale = ft_parse_vec3(line);
	else if (ft_strnequ(line, "col", 3))
		object->color = ft_parse_rgba(line);
	else if (ft_strnequ(line, "rad", 3))
		object->radius = ft_strtod(line + 4);
}

int			parse_object(int fd, t_obj_type type, t_object *object)
{
	char *line;

	if (type < 0 || !object)
		panic("wrong object type!");
	init_object(object);
	while (ft_get_next_line(fd, &line))
	{
		object->type = type;
		if (line[0] == '#')
		{
			if (type == CYLINDER)
				rotate_cylinder(object, object->rotation);
			if (type == CONE)
				rotate_cone(object, object->rotation);
			if (type == PLANE)
				rotate_plane(object, object->rotation);
			free(line);
			return (1);
		}
		else
			set_object_property(line, object);
		free(line);
	}
	return (0);
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
