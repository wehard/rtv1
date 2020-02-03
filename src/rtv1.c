/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 18:54:01 by wkorande         ###   ########.fr       */
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
#include <pthread.h>

static void print_vec3(t_vec3 v)
{
	ft_printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z);
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

void	draw_lights(t_env *env, t_scene *scene)
{
	int		i;
	t_vec2i	p;

	i = 0;
	while (i < scene->num_lights)
	{
		p = world_to_screen_point(&scene->camera, scene->lights[i].position);
		mlx_string_put(env->mlx->mlx_ptr, env->mlx->win_ptr, p.x, p.y,
			ft_get_color(scene->lights[i].color),
			scene->lights[i].type == DIRECTIONAL ? "D" : "P");
		i++;
	}
}

void render(t_env *env, t_scene *scene)
{
	clock_t start, end;
	double cpu_time_used;
	t_ray 			ray;
	t_raycasthit	hit;
	t_vec2i			cur;
	t_vec2			screen;

	init_camera(&scene->camera, scene->camera.pos, scene->camera.look_at,
		scene->camera.fov, scene->camera.aspect);
	start = clock();
	cur.y = 0;
	while (cur.y < env->height)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			screen.x = (double)cur.x / (double)WIN_W;
			screen.y = (double)cur.y / (double)WIN_H;
			ray = get_camera_ray(&scene->camera, screen.x, screen.y);
			put_pixel_mlx_img(env->mlx_img, cur.x, cur.y,
				ft_get_color(raycast(&ray, scene, &hit)));
			cur.x++;
		}
		cur.y++;
	}
	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx_img->img, 0, 0);
	draw_lights(env, scene);
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
	render(env, env->scene);
	mlx_hook(env->mlx->win_ptr, 2, (1L << 0), key_press, (void*)env);
	mlx_expose_hook(env->mlx->win_ptr, update, (void*)env);
	//mlx_hook(env->mlx->win_ptr, 4, (1L << 2), mouse_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
