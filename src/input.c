/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:58:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 15:51:22 by wkorande         ###   ########.fr       */
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

static void handle_object_rotation(t_object *object, int key)
{
	if (key == KEY_NUM_7)
		object->rotation.z -= 1.0;
	if (key == KEY_NUM_9)
		object->rotation.z += 1.0;
	if (key == KEY_NUM_1)
		object->rotation.x -= 1.0;
	if (key == KEY_NUM_3)
		object->rotation.x += 1.0;
	if (object->type == CYLINDER)
		rotate_cylinder(object, object->rotation);
	if (object->type == PLANE)
		rotate_plane(object, object->rotation);
	if (object->type == CONE)
		rotate_cone(object, object->rotation);
}

static void	handle_object_manipulation(t_object *object, int key)
{
	if (key == KEY_NUM_LEFT)
		object->position.x -= 1.0;
	if (key == KEY_NUM_RIGHT)
		object->position.x += 1.0;
	if (key == KEY_NUM_UP)
		object->position.z -= 1.0;
	if (key == KEY_NUM_DOWN)
		object->position.z += 1.0;
	if (key == KEY_NUM_PLUS)
		object->position.y += 1.0;
	if (key == KEY_NUM_MINUS)
		object->position.y -= 1.0;
}

int			key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	ft_printf("%d\n", key);
	if (key == KEY_ESC)
		del_env_exit(env);
	if (key == KEY_SPACE)
		update(param);
	handle_camera_movement(env, key);
	if (env->scene->selected_object)
	{
		handle_object_manipulation(env->scene->selected_object, key);
		handle_object_rotation(env->scene->selected_object, key);
		print_object_info(env->scene->selected_object);
	}
	render(env, env->scene);
	return (0);
}

int			mouse_press(int button, int x, int y, void *param)
{
	t_env *env;
	t_ray ray;
	t_raycasthit hit;
	t_vec2	screen;

	env = (t_env*)param;
	if (button == 1)
	{
		ft_printf("mouse pos: %d, %d\n", x, y);
		screen.x = (double)x / (double)WIN_W;
		screen.y = (double)y / (double)WIN_H;
		ft_printf("screen pos: %.3f, %.3f\n", screen.x, screen.y);
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
	return (1);
}

