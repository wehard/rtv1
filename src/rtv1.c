/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 15:14:59 by wkorande         ###   ########.fr       */
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

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	ft_printf("key: %d\n", key);
	if (key == KEY_ESC)
		del_env_exit(env);
	if (key == KEY_SPACE)
		update(param);
	return (0);
}

int	raycast_shadow(t_scene *scene, t_raycasthit *origin)
{
	t_ray			ray;
	t_raycasthit	hit;
	int				i;

	ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(origin->normal, SHADOW_BIAS));
	ray.direction = ft_normalize_vec3(ft_sub_vec3(scene->light.position, ray.origin));
	i = 0;
	while (i < scene->num_shapes)
	{
		if (origin->shape != &scene->shapes[i] && intersects_shape(&ray, &scene->shapes[i], &hit))
			return (1);
		i++;
	}
	return (0);
}

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth)
{
	double			min_dist = INFINITY;
	t_shape			*cur_shape;
	t_raycasthit	cur_hit;
	int 			hit_found;
	int				i;

	if (depth > MAX_RAY_DEPTH)
	{
		hit->hit_color = scene->ambient_color;
		return (0);
	}

	hit_found = 0;
	i = 0;
	while (i < scene->num_shapes)
	{
		cur_shape = &scene->shapes[i];

		if (intersects_shape(ray, cur_shape, &cur_hit))
		{
			hit_found = 1;
			if (cur_hit.distance < min_dist)
			{
				*hit = cur_hit;
				min_dist = cur_hit.distance;
			}
		}
		i++;
	}
	if (hit_found)
	{
		hit->hit_color = hit->shape->color;
		if (depth == 0 && raycast_shadow(scene, hit))
			hit->hit_color = ft_mul_rgba(hit->hit_color, 0.3);
		if (hit->shape->reflect > 0)
		{
			t_ray reflect_ray;
			t_raycasthit reflect_hit;
			reflect_ray.origin = ft_add_vec3(hit->point, ft_mul_vec3(hit->normal, 0.001f));
			reflect_ray.direction = ft_reflect_vec3(ray->direction, hit->normal);
			if (raycast(&reflect_ray, scene, &reflect_hit, depth + 1))
				hit->hit_color = ft_lerp_rgba(hit->hit_color, reflect_hit.hit_color, hit->shape->reflect);
		}

	}
	else
		hit->hit_color = scene->ambient_color;
	return (hit_found);
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
		ft_putendl("scene modified");
		read_scene(env->scene, env->scene->path);
		render(env, env->scene);
	}
	return (0);
}

void render(t_env *env, t_scene *scene)
{
	clock_t start, end;
	double cpu_time_used;
	t_ray ray;
	t_raycasthit hit;


	clear_mlx_img(env->mlx_img);


	double scale = tan((scene->fov * 0.5f) * M_PI / 180.0f);
    double aspect_ratio = (double)env->width / (double)env->height;

	start = clock();
	int y = 0;
	int x = 0;
	while (y < env->height)
	{
		x = 0;
		while (x < env->width)
		{
			double rx = (2 * (x + 0.5) / (double)env->width - 1) * aspect_ratio * scale;
            double ry = (1 - 2 * (y + 0.5) / (double)env->height) * scale;

			ray.origin = ft_make_vec3(0.0, 0, 0.0);
			ray.direction = ft_normalize_vec3(ft_sub_vec3(ft_make_vec3(rx, ry, 1.0), ray.origin));
			t_rgba color;
			if (raycast(&ray, scene, &hit, 0))
			{
				color = scene->ambient_color;
				t_vec3 l_dir = ft_normalize_vec3(ft_sub_vec3(scene->light.position, hit.point));
				double light_dot_normal = ft_dot_vec3(l_dir, hit.normal); //-raydir.dot(nhit);
				double ray_dot_normal = ft_dot_vec3(ray.direction, hit.normal);
				double specular = ft_lerp_f(pow(light_dot_normal, 200), 1, 0.5);
				color = ft_lerp_rgba(scene->ambient_color, hit.hit_color, ft_max_d(0.0, ray_dot_normal));
				color = ft_lerp_rgba(color, hit.hit_color, ft_max_d(0.0, light_dot_normal));
				color = ft_mul_rgba(color, specular);
			}
			else
				color = hit.hit_color;
			put_pixel_mlx_img(env->mlx_img, x, y, ft_get_color(color));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx_img->img, 0, 0);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	ft_printf("rendered in: %fs\n", cpu_time_used);
}

int	main(int argc, char **argv)
{
	t_env *env;
	char window_title[50];

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
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
