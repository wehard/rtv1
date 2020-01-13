/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/13 21:17:25 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>
#include <mlx.h>
#include <math.h>
#include <time.h>
//#include "ray.h"
//#include "vector.h"
#include "libft.h"
#include "ft_printf.h"
#include "keys.h"

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	ft_printf("key: %d\n", key);
	if (key == KEY_ESC)
		del_env_exit(env);
	return (0);
}

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *closest_hit)
{
	//t_ray shadow_ray;
	//float shadow_bias = 0.0001f;
	float min_dist = INFINITY;
	t_shape *current_shape;
	t_raycasthit hit;
	int hitfound = 0;
	int i = 0;
	while (i < scene->num_shapes)
	{
		current_shape = &scene->shapes[i];
		if (ray->is_shadow_ray && ray->origin_shape == current_shape)
		{
			i++;
			continue;
		}
		if (intersects_shape(ray, current_shape, &hit))
		{
			hitfound = 1;
			if (ray->is_shadow_ray)
				return (hitfound);
			if (hit.distance < min_dist)
			{
				*closest_hit = hit;
				min_dist = hit.distance;
			}
		}
		i++;
	}
	/*
	int is_shadow = 0;
	if (closest_shape != NULL)
	{
		t_raycasthit shadow_hit;

		int j = 0;
		min_dist = INFINITY;
		while (j < scene->num_shapes)
		{
			if (shadow_ray.origin_shape == &scene->shapes[j])
			{
				j++;
				continue;
			}
			if (intersects_shape(&shadow_ray, &scene->shapes[j], &shadow_hit))
			{
				is_shadow = 1;
				break ;
			}
			j++;
		}
		t_vec3 light_dir = normalize_vec3(sub_vec3(scene->light.position, closest_hit.point));
		diffuse = mul_vec3(closest_shape->color, dot_vec3(closest_normal, light_dir));
		if (is_shadow)
			diffuse = mul_vec3(diffuse, 0.4f);
	}
	*/
	return (hitfound);
}

void render_scene(t_env *env, t_scene *scene)
{
	t_ray ray;
	t_raycasthit hit;
	t_vec3 color = make_vec3(0,0,0);

	scene->light.position = make_vec3(-5.0f, 20.0f, 5.0f);
	scene->light.intensity = 1.0f;

	float fov = 60.0f;
	float scale = tan((fov * 0.5f) * M_PI / 180.0f);
    float imageAspectRatio = (float)env->width / (float)env->height;

	int y = 0;
	int x = 0;
	while (y < env->height)
	{
		x = 0;
		while (x < env->width)
		{
			float rx = (2 * (x + 0.5) / (float)env->width - 1) * imageAspectRatio * scale;
            float ry = (1 - 2 * (y + 0.5) / (float)env->height) * scale;

			ray.is_shadow_ray = 0;
			ray.origin = make_vec3(0.0, 0.0, 0.0);
			ray.direction = normalize_vec3(sub_vec3(make_vec3(rx, ry, 1.0), ray.origin));

			if (raycast(&ray, scene, &hit))
			{
				t_vec3 light_dir = normalize_vec3(sub_vec3(scene->light.position, hit.point));
				color = mul_vec3(hit.hit_shape->color, dot_vec3(hit.normal, light_dir));
				if (hit.hit_shape->reflect > 0.0)
				{
					t_ray reflect_ray;
					t_raycasthit reflect_hit;
					reflect_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, 0.001f));
					reflect_ray.direction = reflect_vec3(ray.direction, hit.normal);
					reflect_ray.is_shadow_ray = 0;
					if (raycast(&reflect_ray, scene, &reflect_hit))
					{
						color = add_vec3(color, mul_vec3(reflect_hit.hit_shape->color, hit.hit_shape->reflect));
					}
				}
				t_ray shadow_ray;
				t_raycasthit shadow_hit;
				float shadow_bias = 0.001f;
				shadow_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, shadow_bias));
				shadow_ray.direction = normalize_vec3(sub_vec3(scene->light.position, shadow_ray.origin));
				shadow_ray.origin_shape = hit.hit_shape;
				shadow_ray.is_shadow_ray = 1;
				if (raycast(&shadow_ray, scene, &shadow_hit))
					color = mul_vec3(color, 0.4f);

				put_pixel_mlx_img(env->mlx_img, x, y, ft_get_color(color));
			}
			x++;
		}
		y++;
	}
}

int	main(int argc, char **argv)
{
	t_env *env;
	clock_t start, end;
	double cpu_time_used;

	if (argc != 2)
	{
		ft_printf("usage: ./RTv1 <scene file>\n");
		return (1);
	}

	env = init_env(WIN_W, WIN_H, "RTv1");

	t_scene scene;
	scene.shapes = read_scene(argv[1], &scene.num_shapes);

	start = clock();

	mlx_string_put(env->mlx->mlx_ptr, env->mlx->win_ptr, WIN_W/2, WIN_H/2, 0xFFFFFF, "rendering");
	clear_mlx_img(env->mlx_img);

	render_scene(env, &scene);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	ft_printf("rendered in: %fs\n", cpu_time_used);

	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx_img->img, 0, 0);

	mlx_hook(env->mlx->win_ptr, 2, (1L<<0), key_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
