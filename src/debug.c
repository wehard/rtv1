/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 12:45:18 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 18:44:14 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "ft_printf.h"

void	print_vec3(t_vec3 v)
{
	ft_printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z);
}

void	print_object_info(t_object *object)
{
	ft_printf("type: %d\n", object->type);
	ft_printf("pos: %.3f, %.3f, %.3f\n", object->position.x,
		object->position.y, object->position.z);
	ft_printf("rot: %.3f, %.3f, %.3f\n", object->rotation.x,
		object->rotation.y, object->rotation.z);
	ft_printf("scale: %.3f, %.3f, %.3f\n", object->scale.x,
		object->scale.y, object->scale.z);
	ft_printf("normal: %.3f, %.3f, %.3f\n", object->normal.x,
		object->normal.y, object->normal.z);
	ft_printf("color: %.3f, %.3f, %.3f, %.3f\n", object->color.r,
		object->color.g, object->color.b, object->color.a);
	ft_printf("start: %.3f, %.3f, %.3f\n", object->start.x,
		object->start.y, object->start.z);
	ft_printf("end: %.3f, %.3f, %.3f\n", object->end.x,
		object->end.y, object->end.z);
	ft_printf("radius: %.3f\n", object->radius);
}
