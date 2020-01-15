/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/15 17:20:16 by wkorande         ###   ########.fr       */
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

int	raycast_shadow(t_scene *scene, t_raycasthit *origin)
{
	t_ray			ray;
	t_raycasthit	hit;
	int				i;

	ray.origin = add_vec3(origin->point, mul_vec3(origin->normal, SHADOW_BIAS));
	ray.direction = normalize_vec3(sub_vec3(scene->light.position, ray.origin));
	i = 0;
	while (i < scene->num_shapes)
	{
		if (origin->shape != &scene->shapes[i] && intersects_shape(&ray, &scene->shapes[i], &hit))
			return (1);
		i++;
	}
	return (0);
}

t_vec3	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	t_vec3			color;
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
		color = hit->shape->color;
		if (raycast_shadow(scene, hit))
			color = mul_vec3(hit->shape->color, 0.2);
	}
	else
		color = scene->ambient_color;
	return (color);
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

	scene->light.intensity = 1.0f;
	scene->light.color = make_vec3(1.0, 1.0, 1.0);

	float fov = 90.0f;
	float scale = tan((fov * 0.5f) * M_PI / 180.0f);
    float aspect_ratio = (float)env->width / (float)env->height;

	start = clock();
	int y = 0;
	int x = 0;
	while (y < env->height)
	{
		x = 0;
		while (x < env->width)
		{
			float rx = (2 * (x + 0.5) / (float)env->width - 1) * aspect_ratio * scale;
            float ry = (1 - 2 * (y + 0.5) / (float)env->height) * scale;

			ray.origin = make_vec3(0.0, 0.0, 0.0);
			ray.direction = normalize_vec3(sub_vec3(make_vec3(rx, ry, 1.0), ray.origin));

			t_vec3 color;
			color = raycast(&ray, scene, &hit);
			t_vec3 l_dir = normalize_vec3(sub_vec3(scene->light.position, hit.point));
			float ratio = dot_vec3(l_dir, hit.normal); //-raydir.dot(nhit);
        	float fresnel =  ft_lerp_f(pow(ratio, 3), 1, 0.144);

			color = mul_vec3(color, fresnel);
			//t_vec3 light_dir = normalize_vec3(sub_vec3(scene->light.position, hit.point));
			//diffuse = mul_vec3(hit.shape->color, dot_vec3(hit.normal, light_dir));
			//if (raycast_shadow(scene, &hit))
			//	diffuse = mul_vec3(diffuse, 0.1);
			//if (hit.shape->reflect > 0.0)
			//{
			//	t_ray reflect_ray;
			//	t_raycasthit reflect_hit;
			//	reflect_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, 0.001f));
			//	reflect_ray.direction = reflect_vec3(ray.direction, hit.normal);
			//	if (raycast(&reflect_ray, scene, &reflect_hit, FALSE))
			//		diffuse = add_vec3(diffuse, mul_vec3(reflect_hit.shape->color, hit.shape->reflect));
			//}
			//float specular = ft_max_d(0.0, dot_vec3(light_dir, reflect_vec3(ray.direction, hit.normal))) * 0.5;
			//diffuse = add_vec3(diffuse, make_vec3(specular, specular, specular));
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

	if (argc != 2)
	{
		ft_printf("usage: ./RTv1 <scene file>\n");
		return (1);
	}
	env = init_env(WIN_W, WIN_H, "RTv1");
	read_scene(env->scene, argv[1]);
	mlx_string_put(env->mlx->mlx_ptr, env->mlx->win_ptr, WIN_W/2, WIN_H/2, 0xFFFFFF, "rendering");
	render(env, env->scene);
	mlx_hook(env->mlx->win_ptr, 2, (1L << 0), key_press, (void*)env);
	mlx_expose_hook(env->mlx->win_ptr, update, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
