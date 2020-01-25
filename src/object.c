/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 15:34:42 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 15:12:31 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "ft_printf.h"

void print_object_info(t_object *object)
{
	ft_printf("type: %d\n", object->type);
	ft_printf("pos: %.3f, %.3f, %.3f\n", object->position.x, object->position.y, object->position.z);
	ft_printf("rot: %.3f, %.3f, %.3f\n", object->rotation.x, object->rotation.y, object->rotation.z);
	ft_printf("scale: %.3f, %.3f, %.3f\n", object->scale.x, object->scale.y, object->scale.z);
	ft_printf("normal: %.3f, %.3f, %.3f\n", object->normal.x, object->normal.y, object->normal.z);
	ft_printf("color: %.3f, %.3f, %.3f, %.3f\n", object->color.r, object->color.g, object->color.b, object->color.a);
	ft_printf("radius: %.3f\n", object->radius);
	ft_printf("reflect: %.3f\n", object->reflect);
}

t_vec3	calculate_hit_normal(t_raycasthit *hit)
{
	if (!hit || !hit->object)
		return (ft_make_vec3(0, 0, 0));
	if (hit->object->type == PLANE)
		return (hit->normal);
	if (hit->object->type == SPHERE)
		return (hit->normal);
	if (hit->object->type == BOX)
		return (hit->normal);
	if (hit->object->type == CYLINDER)
		return (hit->normal);
	if (hit->object->type == CONE)
		return (hit->normal);
	return (hit->object->normal);
}

int	intersects_object(t_ray *ray, t_object *object, t_raycasthit *hit)
{
	int hit_found;

	hit_found = 0;
	hit->distance = 0.0f;
	if (object->type == PLANE)
		hit_found = intersects_plane(ray, object, hit);
	else if (object->type == DISC)
		hit_found = intersects_disc(ray, object, hit);
	else if (object->type == SPHERE)
		hit_found = intersects_sphere(ray, object, hit);
	//else if (object->type == BOX)
	//	hit_found = intersects_box(ray, object, hit);
	else if (object->type == CYLINDER)
		hit_found = intersects_cylinder(ray, object, hit);
	else if (object->type == CONE)
		hit_found = intersects_cone(ray, object, hit);
	if (hit_found)
	{
		hit->object = object;
		hit->point = point_on_ray(ray, hit->t);
		hit->distance = hit->t;
		hit->normal = calculate_hit_normal(hit);
	}
	return (hit_found);
}
