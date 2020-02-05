/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:58:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 12:15:59 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "keys.h"
#include "ft_printf.h"

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	if (key == KEY_ESC)
		del_env_exit(env);
	if (key == KEY_SPACE)
		update(param);
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
	render(env, env->scene);
	return (0);
}


int		mouse_press(int button, int x, int y, void *param)
{
	t_env *env;
	t_ray ray;
	t_raycasthit hit;

	env = (t_env*)param;
	if (button == 1 || button == 2)
	{
		ray = get_camera_ray(&env->scene->camera, x, y);
		print_vec3(ray.direction);
		hit.object = NULL;
		raycast(&ray, env->scene, &hit);
		if (hit.object != NULL)
		{
			ft_printf("hitinfo\n");
			ft_printf("%-10s %.3f, %.3f, %.3f\n", "point:", hit.point.x, hit.point.y, hit.point.z);
			ft_printf("%-10s %.3f, %.3f, %.3f\n", "normal:", hit.normal.x, hit.normal.y, hit.normal.z);
			ft_printf("\n");
			t_vec2i sp = world_to_screen_point(&env->scene->camera, hit.point);
			t_vec2i np = world_to_screen_point(&env->scene->camera, ft_add_vec3(hit.point, hit.normal));
			draw_line(env->mlx, ft_make_vec3(sp.x, sp.y, 0), ft_make_vec3(np.x, np.y, 0), 0x00FFFF);
			if (button == 2)
				env->scene->camera.look_at = hit.point;
		}
		else
			ft_printf("No hit!\n");
	}
	return (1);
}

