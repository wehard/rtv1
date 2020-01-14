/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/14 19:09:40 by wkorande         ###   ########.fr       */
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

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *closest_hit, int shadow_ray)
{
	float			min_dist = INFINITY;
	t_shape			*cur_shape;
	t_raycasthit	cur_hit;
	int 			hit_found;
	int				i;

	hit_found = 0;
	i = 0;
	while (i < scene->num_shapes)
	{
		cur_shape = &scene->shapes[i];
		if (shadow_ray && ray->origin_shape == cur_shape)
		{
			i++;
			continue;
		}
		if (intersects_shape(ray, cur_shape, &cur_hit))
		{
			hit_found = 1;
			if (cur_hit.distance < min_dist)
			{
				*closest_hit = cur_hit;
				if (shadow_ray)
					return (hit_found);
				min_dist = cur_hit.distance;
			}
		}
		i++;
	}
	return (hit_found);
}

void render_scene(t_env *env, t_scene *scene)
{
	t_ray ray;
	t_raycasthit hit;
	t_vec3 diffuse = make_vec3(0.6,0.6,1.0);

	scene->light.position = make_vec3(-1.0f, 20.0f, 5.0f);
	scene->light.intensity = 1.0f;
	scene->light.color = make_vec3(1.0, 1.0, 1.0);
	scene->ambient_color = make_vec3(0.5, 0.5, 0.5);

	float fov = 90.0f;
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

			ray.origin = make_vec3(0.0, 0.0, 0.0);
			ray.direction = normalize_vec3(sub_vec3(make_vec3(rx, ry, 1.0), ray.origin));

			if (raycast(&ray, scene, &hit, FALSE))
			{
				t_vec3 light_dir = normalize_vec3(sub_vec3(scene->light.position, hit.point));
				diffuse = mul_vec3(hit.hit_shape->color, dot_vec3(hit.normal, light_dir));
				if (hit.hit_shape->reflect > 0.0)
				{
					t_ray reflect_ray;
					t_raycasthit reflect_hit;
					reflect_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, 0.001f));
					reflect_ray.direction = reflect_vec3(ray.direction, hit.normal);
					if (raycast(&reflect_ray, scene, &reflect_hit, FALSE))
					{
						diffuse = add_vec3(diffuse, mul_vec3(reflect_hit.hit_shape->color, hit.hit_shape->reflect));
					}
				}
				t_ray shadow_ray;
				t_raycasthit shadow_hit;
				float shadow_bias = 0.001f;
				shadow_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, shadow_bias));
				shadow_ray.direction = normalize_vec3(sub_vec3(scene->light.position, shadow_ray.origin));
				shadow_ray.origin_shape = hit.hit_shape;
				if (raycast(&shadow_ray, scene, &shadow_hit, TRUE))
					diffuse = mul_vec3(diffuse, 0.4f);
				float specular = ft_max_d(0.0, dot_vec3(light_dir, reflect_vec3(ray.direction, hit.normal))) * 0.5;
				diffuse = add_vec3(diffuse, make_vec3(specular, specular, specular));
				t_vec3 color = diffuse;
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
