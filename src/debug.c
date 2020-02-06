/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 12:45:18 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 16:25:19 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "mlx.h"
#include "ft_printf.h"

void		print_vec3(t_vec3 v)
{
	ft_printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z);
}

void		print_object_info(t_object *object)
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

static void	display_light_info(t_mlx *mlx, t_light *l)
{
	char buf[150];

	ft_bzero(buf, 150);
	ft_sprintf(buf, "light: %p type: %d int: %.3f pos: %.3f, %.3f, %.3f",
		l, l->type, l->intensity, l->position.x, l->position.y, l->position.z);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 10, WIN_H - 25, 0xFFFFFF, buf);
}

static void	display_object_info(t_mlx *mlx, t_object *object)
{
	char buf[150];

	ft_bzero(buf, 150);
	ft_sprintf(buf, "object: %p pos: %.1f, %.1f, %.1f rot: %.1f, %.1f, %.1f",
		object, object->position.x, object->position.y, object->position.z,
		object->rotation.x, object->rotation.y, object->rotation.z);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 10, WIN_H - 25, 0xFFFFFF, buf);
}

void		draw_debug_info(t_env *env, t_scene *scene)
{
	t_mlx	*mlx;
	char	buf[150];

	mlx = env->mlx;
	ft_bzero(buf, 150);
	ft_sprintf(buf, "num_lights: %d", scene->num_lights);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 10, 5, 0xFFFFFF, buf);
	ft_bzero(buf, 150);
	ft_sprintf(buf, "num_objects: %d", scene->num_objects);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 10, 20, 0xFFFFFF, buf);
	ft_bzero(buf, 150);
	ft_sprintf(buf, "threads: %d", NUM_THREADS);
	mlx_string_put(mlx->mlx_ptr, mlx->win_ptr, 10, 35, 0xFFFFFF, buf);
	if (scene->selected_light)
		display_light_info(env->mlx, scene->selected_light);
	else if (scene->selected_object)
		display_object_info(env->mlx, scene->selected_object);
}
