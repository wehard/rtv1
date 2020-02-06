/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:58:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 12:53:24 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "keys.h"
#include "ft_printf.h"

static void	handle_camera_movement(t_env *env, int key)
{
	if (key == KEY_LEFT)
	{
		env->scene->camera.look_at.x -= 1.0;
		env->scene->camera.pos.x -= 1.0;
	}
	if (key == KEY_RIGHT)
	{
		env->scene->camera.look_at.x += 1.0;
		env->scene->camera.pos.x += 1.0;
	}
	if (key == KEY_UP)
	{
		env->scene->camera.pos.z -= 1.0;
		env->scene->camera.look_at.z -= 1.0;
	}
	if (key == KEY_DOWN)
	{
		env->scene->camera.pos.z += 1.0;
		env->scene->camera.look_at.z += 1.0;
	}
}

static void	handle_object_manipulation(t_object *o, int key)
{
	if (key == KEY_NUM_7 || key == KEY_NUM_9)
		key == KEY_NUM_7 ? (o->rotation.z -= 1.0) : (o->rotation.z += 1.0);
	if (key == KEY_NUM_1 || key == KEY_NUM_3)
		key == KEY_NUM_1 ? (o->rotation.x -= 1.0) : (o->rotation.x += 1.0);
	if (o->type == CYLINDER)
		rotate_cylinder(o, o->rotation);
	if (o->type == PLANE)
		rotate_plane(o, o->rotation);
	if (o->type == CONE)
		rotate_cone(o, o->rotation);
	if (key == KEY_NUM_LEFT || key == KEY_NUM_RIGHT)
		key == KEY_NUM_LEFT ? (o->position.x -= 1.0) : (o->position.x += 1.0);
	if (key == KEY_NUM_UP || key == KEY_NUM_DOWN)
		key == KEY_NUM_UP ? (o->position.z -= 1.0) : (o->position.z += 1.0);
	if (key == KEY_NUM_PLUS || key == KEY_NUM_MINUS)
		key == KEY_NUM_PLUS ? (o->position.y += 1.0) : (o->position.y -= 1.0);
}

static void	handle_light_manipulation(t_light *l, int key)
{
	if (key == KEY_NUM_MULT)
		l->intensity += 0.1;
	if (key == KEY_NUM_DIV)
		l->intensity -= 0.1;
	if (l->type == DIRECTIONAL)
	{
		if (key == KEY_NUM_8 || key == KEY_NUM_2)
			key == KEY_NUM_8 ? (l->rotation.x += 1.0) : (l->rotation.x -= 1.0);
		if (key == KEY_NUM_4 || key == KEY_NUM_6)
			key == KEY_NUM_4 ? (l->rotation.z += 1.0) : (l->rotation.z -= 1.0);
		rotate_light(l, l->rotation);
	}
	else
	{
		if (key == KEY_NUM_4 || key == KEY_NUM_6)
			key == KEY_NUM_4 ? (l->position.x -= 1.0) : (l->position.x += 1.0);
		if (key == KEY_NUM_8 || key == KEY_NUM_2)
			key == KEY_NUM_8 ? (l->position.z -= 1.0) : (l->position.z += 1.0);
		if (key == KEY_NUM_PLUS || key == KEY_NUM_MINUS)
			key == KEY_NUM_PLUS ? (l->position.y += 1.0) :
				(l->position.y -= 1.0);
	}
}

int			key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	if (key == KEY_ESC || key == KEY_SPACE)
		key == KEY_ESC ? del_env_exit(env) : update(param);
	if (key == KEY_I)
		env->debug = !env->debug;
	handle_camera_movement(env, key);
	if (key >= KEY_1 && key <= KEY_3)
	{
		env->scene->selected_object = NULL;
		if (key - KEY_1 < env->scene->num_lights)
			env->scene->selected_light = &env->scene->lights[key - KEY_1];
	}
	if (env->scene->selected_object)
	{
		env->scene->selected_light = NULL;
		handle_object_manipulation(env->scene->selected_object, key);
	}
	if (env->scene->selected_light)
		handle_light_manipulation(env->scene->selected_light, key);
	render(env, env->scene);
	return (0);
}

int			mouse_press(int button, int x, int y, void *param)
{
	t_hit	hit;
	t_env	*env;
	t_ray	ray;
	t_vec2	screen;

	env = (t_env*)param;
	if (button == 1)
	{
		screen.x = (double)x / (double)WIN_W;
		screen.y = (double)y / (double)WIN_H;
		ray = get_camera_ray(&env->scene->camera, screen.x, screen.y);
		hit.object = NULL;
		if (trace(&ray, env->scene, &hit, FALSE) && hit.object != NULL)
		{
			env->scene->selected_object = hit.object;
			print_object_info(hit.object);
		}
		else
		{
			ft_printf("No hit!\n");
			env->scene->selected_object = NULL;
		}
	}
	refresh(env);
	return (1);
}
