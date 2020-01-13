/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 15:34:42 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/13 19:36:13 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "ft_printf.h"

void print_shape_info(t_shape *shape)
{
	ft_printf("type: %d\n", shape->type);
	ft_printf("pos: %.3f, %.3f, %.3f\n", shape->position.x, shape->position.y, shape->position.z);
	ft_printf("rot: %.3f, %.3f, %.3f\n", shape->rotation.x, shape->rotation.y, shape->rotation.z);
	ft_printf("scale: %.3f, %.3f, %.3f\n", shape->scale.x, shape->scale.y, shape->scale.z);
	ft_printf("normal: %.3f, %.3f, %.3f\n", shape->normal.x, shape->normal.y, shape->normal.z);
	ft_printf("color: %.3f, %.3f, %.3f\n", shape->color.x, shape->color.y, shape->color.z);
	ft_printf("radius: %.3f\n", shape->radius);
	ft_printf("reflect: %.3f\n", shape->reflect);
}

t_vec3	hit_normal(t_raycasthit *hit, t_shape *shape)
{
	if (shape->type == PLANE)
		return (shape->normal);
	if (shape->type == SPHERE)
		return (normalize_vec3(sub_vec3(hit->point, shape->position)));
	if (shape->type == BOX)
		return (hit->normal);
	return (hit->normal);
}

int	intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit)
{
	int hit_found;

	hit_found = 0;
	hit->distance = 0.0f;
	if (shape->type == PLANE)
		hit_found = intersects_plane(ray, shape, hit);
	if (shape->type == SPHERE)
		hit_found = intersects_sphere(ray, shape, hit);
	if (shape->type == BOX)
		hit_found = intersects_box(ray, shape, hit);
	if (hit_found)
	{
		hit->point = point_on_ray(ray, hit->t);
		hit->distance = len_vec3(add_vec3(ray->origin, hit->point));
		hit->normal = hit_normal(hit, shape);
		hit->hit_shape = shape;
	}
	return (hit_found);
}