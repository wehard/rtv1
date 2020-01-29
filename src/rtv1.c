/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/29 18:28:09 by wkorande         ###   ########.fr       */
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

static void print_vec3(t_vec3 v)
{
	ft_printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z);
}

#define DEBUG 0



static t_vec2i world_to_screen_point(t_camera *camera, t_vec3 wp)
{
	t_vec3 dir;
	t_vec2i sp;
	double aspect = (double)WIN_W / (double)WIN_H;
	dir = ft_normalize_vec3(ft_sub_vec3(wp, camera->pos));
	t_vec3 ipp = ft_mul_vec3(dir, 1.0);
	sp.x = ((ipp.x + aspect * 0.5) / aspect) * WIN_W;
	sp.y =  WIN_H - ((ipp.y + 0.5) * WIN_H);
	return (sp);
}
/*
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
		if (raycast(&ray, env->scene, &hit, 0))
		{
			ft_printf("hitinfo\n");
			ft_printf("%-10s %.3f, %.3f, %.3f\n", "point:", hit.point.x, hit.point.y, hit.point.z);
			ft_printf("%-10s %.3f, %.3f, %.3f\n", "normal:", hit.normal.x, hit.normal.y, hit.normal.z);
			ft_putchar('\n');
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
*/
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
	t_ray 			ray;
	t_raycasthit	hit;
	t_vec2i			cur;
	int seed = 1234;

	clear_mlx_img(env->mlx_img);
	start = clock();
	cur.y = env->height;
	while (cur.y >= 0)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			t_rgba color = ft_make_rgba(0, 0, 0, 1);
			int i = 0;
			while (i < RAYS_PER_PIXEL)
			{
				double u = (double)cur.x / (double)WIN_W;
				double v = (double)cur.y / (double)WIN_H;
				ray = get_camera_ray(&scene->camera, u, v);
				color = ft_lerp_rgba(color, raycast(&ray, scene, &hit, 0), 0.5);
				i++;
			}
			put_pixel_mlx_img(env->mlx_img, cur.x, cur.y, ft_get_color(color));
			cur.x++;
		}
		cur.y--;
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
	//mlx_hook(env->mlx->win_ptr, 4, (1L << 2), mouse_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
