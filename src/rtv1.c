/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 18:42:46 by wkorande         ###   ########.fr       */
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

int		update(void *param)
{
	t_env	*env;
	time_t	newtime;

	env = (t_env*)param;
	if (!param || !env || !env->scene)
		return (1);
	newtime = check_mod_time(env->scene->path);
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

void	*render_thread(void *env_ptr)
{
	t_env			*env;
	t_ray			ray;
	t_vec2			screen;
	t_vec2i			cur;
	t_raycasthit	hit;

	env = (t_env*)env_ptr;
	cur.y = env->thread_index;
	while (cur.y < env->height)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			screen.x = (double)cur.x / (double)WIN_W;
			screen.y = (double)cur.y / (double)WIN_H;
			ray = get_camera_ray(&env->scene->camera, screen.x, screen.y);
			put_pixel_mlx_img(env->mlx_img, cur.x, cur.y,
				ft_get_color(raycast(&ray, env->scene, &hit)));
			cur.x++;
		}
		cur.y += NUM_THREADS;
	}
	return (env_ptr);
}

void	render(t_env *env, t_scene *scene)
{
	pthread_t	threads[NUM_THREADS];
	t_env		thread_env[NUM_THREADS];
	int			i;

	init_camera(&scene->camera, scene->camera.pos, scene->camera.look_at,
		scene->camera.fov);
	i = 0;
	while (i < NUM_THREADS)
	{
		ft_memcpy((void*)&thread_env[i], (void*)env, sizeof(t_env));
		thread_env[i].thread_index = i;
		if (pthread_create(&threads[i], NULL, render_thread,
			&thread_env[i]) != 0)
			panic("thread failed!");
		i++;
	}
	while (i--)
		pthread_join(threads[i], NULL);
	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr,
		env->mlx_img->img, 0, 0);
}

int		main(int argc, char **argv)
{
	t_env	*env;
	char	window_title[50];

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
	mlx_hook(env->mlx->win_ptr, 2, (1L << 0), key_press, (void*)env);
	mlx_expose_hook(env->mlx->win_ptr, update, (void*)env);
	mlx_hook(env->mlx->win_ptr, 4, (1L << 2), mouse_press, (void*)env);
	render(env, env->scene);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
