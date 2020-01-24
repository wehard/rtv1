/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/23 18:00:59 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>
#include <mlx.h>
#include <math.h>
#include <time.h>
#include "libft.h"
#include "ft_printf.h"
#include "keys.h"
#include "vector.h"
#include "matrix.h"

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	ft_printf("key: %d\n", key);
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
		env->scene->camera.pos.z -= 1.0;
	if (key == KEY_DOWN)
		env->scene->camera.pos.z += 1.0;
	render(env, env->scene);
	return (0);
}

int		mouse_press(int button, int x, int y, void *param)
{
	t_env *env;
	t_ray ray;
	t_raycasthit hit;
	t_vec2 r;

	env = (t_env*)param;
	if (button == 1)
	{
		r.x = (2 * (x + 0.5) / (double)env->width - 1) * env->scene->options.aspect * env->scene->options.scale;
		r.y = (1 - 2 * (y + 0.5) / (double)env->height) * env->scene->options.scale;
		ray.origin = env->scene->camera.pos;
		ray.direction = ft_normalize_vec3(ft_mul_dir_vec3_mat4(ft_make_vec3(r.x, r.y, -1.0), env->scene->camera_to_world));
		if (raycast(&ray, env->scene, &hit, 0))
		{
			print_object_info(hit.object);
			ft_printf("hit normal: %.3f, %.3f, %.3f\n", hit.normal.x, hit.normal.y, hit.normal.z);
			ft_printf("hit point: %.3f, %.3f, %.3f\n", hit.point.x, hit.point.y, hit.point.z);
			ft_putchar('\n');
		}
		else
			ft_printf("No hit!\n");
	}
	return (1);
}

int	update(void *param)
{
	t_env *env;

	env = (t_env*)param;
	if (!param || !env || !env->scene)
		return (1);
	time_t newtime = check_mod_time(env->scene->path);
	if (env->scene->mod_time < newtime)
	{
		env->scene->mod_time = newtime;
		ft_printf("scene modified\n");
		if (read_scene(env->scene, env->scene->path))
			render(env, env->scene);
		else
			ft_printf("error: failed to read scene!\n");
	}
	return (0);
}

void render(t_env *env, t_scene *scene)
{
	clock_t start, end;
	double cpu_time_used;
	t_ray ray;
	t_raycasthit hit;
	t_vec2i cur;
	t_vec2 r;
	int done = 0;
	scene->camera_to_world = ft_lookat_mat4(scene->camera.pos, scene->camera.look_at, ft_make_vec3(0,1,0));
	if (!done)
	{
		ft_printf("camera to world matrix\n");
		ft_print_mat4(scene->camera_to_world);
		done =1;
	}
	clear_mlx_img(env->mlx_img);
	scene->options.scale = tan(ft_deg_to_rad((scene->options.fov * 0.5)));
	scene->options.aspect = (double)env->width / (double)env->height;
	start = clock();

	ray.origin = scene->camera.pos; // ft_mul_vec3_mat4(scene->camera.pos, scene->camera_to_world);
	cur.y = 0;
	while (cur.y < env->height)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			r.x = (2 * ((cur.x + 0.5) / (double)env->width) - 1) * scene->options.aspect * scene->options.scale;
			r.y = (1 - 2 * (cur.y + 0.5) / (double)env->height) * scene->options.scale;
			ray.direction = ft_normalize_vec3(ft_mul_dir_vec3_mat4(ft_make_vec3(r.x, r.y, -1.0), scene->camera_to_world));
			raycast(&ray, scene, &hit, 0);
			put_pixel_mlx_img(env->mlx_img, cur.x, cur.y, ft_get_color(hit.color));
			cur.x++;
		}
		cur.y++;
	}
	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx_img->img, 0, 0);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	ft_printf("scene rendered in: %fs\n", cpu_time_used);
}

int	main(int argc, char **argv)
{
	t_env *env;
	char window_title[50];
	ft_bzero(window_title, 50);
	if (argc != 2)
	{
		ft_printf("usage: ./RTv1 <scene file>\n");
		return (1);
	}
	ft_sprintf(window_title, "RTv1 - %s", argv[1]);
	env = init_env(WIN_W, WIN_H, window_title);
	if (!read_scene(env->scene, argv[1]))
		return (1);
	mlx_string_put(env->mlx->mlx_ptr, env->mlx->win_ptr, WIN_W/2, WIN_H/2, 0xFFFFFF, "rendering");
	render(env, env->scene);
	mlx_hook(env->mlx->win_ptr, 2, (1L << 0), key_press, (void*)env);
	mlx_expose_hook(env->mlx->win_ptr, update, (void*)env);
	mlx_hook(env->mlx->win_ptr, 4, 0, mouse_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
