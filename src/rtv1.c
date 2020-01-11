/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 13:58:40 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <mlx.h>
#include <math.h>
#include <time.h>
#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"
#include "ray.h"
#include "keys.h"

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	if (key == KEY_ESC)
		del_env_exit(env);
	return (0);
}

t_vec3	hit_normal(t_vec3 hit_point, t_shape *shape)
{
	if (shape->type == PLANE)
		return (shape->normal);
	if (shape->type == SPHERE)
		return (normalize_vec3(sub_vec3(hit_point, shape->position)));
	return (hit_point);
}

int	intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit)
{
	int hit_found;

	hit_found = 0;
	if (shape->type == SPHERE)
		hit_found = intersects_sphere(ray, shape, hit);
	if (shape->type == PLANE)
		hit_found = intersects_plane(ray, shape, hit);
	if (hit_found)
	{
		hit->point = point_on_ray(ray, hit->t);
		hit->distance = len_vec3(sub_vec3(hit->point, ray->origin));
		hit->normal = hit_normal(hit->point, shape);
	}
	if (hit_found && hit->distance > MAX_DISTANCE)
		return (0);
	return (hit_found);
}

int	main(void)
{
	t_env *env;
	clock_t start, end;
	double cpu_time_used;
	env = init_env(1280, 720, "RTv1");

	int num_shapes = 5;
	t_shape shapes[num_shapes];
	shapes[0].type = PLANE;
	shapes[0].position = make_vec3(0.0, -3.0, 5.0);
	shapes[0].color = make_vec3(0.7f, 0.6f, 0.3f);
	shapes[0].normal = make_vec3(0.0f, 1.0f, 0.0f);

	shapes[1].type = SPHERE;
	shapes[1].position = make_vec3(0.0, 4.0, 20.0);
	shapes[1].radius = 1.0f;
	shapes[1].color = make_vec3(1.0f, 0.0f, 0.0f);

	shapes[2].type = SPHERE;
	shapes[2].position = make_vec3(0.0, 0.0, 20.0);
	shapes[2].radius = 3.0f;
	shapes[2].color = make_vec3(0.0f, 1.0f, 0.0f);

	shapes[3].type = SPHERE;
	shapes[3].position = make_vec3(5.0, -1.0, 20.0);
	shapes[3].radius = 1.0f;
	shapes[3].color = make_vec3(0.0f, 0.0f, 1.0f);

	shapes[4].type = SPHERE;
	shapes[4].position = make_vec3(-5.0, -1.0, 20.0);
	shapes[4].radius = 1.0f;
	shapes[4].color = make_vec3(1.0f, 1.0f, 0.0f);



	t_light light;
	light.position = make_vec3(0.0f, 15.0f, 17.0f);
	light.intensity = 1.0f;

	t_ray ray;
	ray.origin = make_vec3(0.0, 0.0, 0.0);
	ray.direction = make_vec3(0.0, 0.0, 1.0);

	t_ray shadow_ray;
	float shadow_bias = 0.0001f;

	float fov = 45.0f;
	float scale = tan((fov * 0.5f) * M_PI / 180.0f);
    float imageAspectRatio = (float)env->width / (float)env->height;

	start = clock();

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
			float minDist = 1000.0f;
			int i = 0;
			t_shape *object = NULL;
			t_raycasthit hit;
			while (i < num_shapes)
			{
				if (intersects_shape(&ray, &shapes[i], &hit))
				{
					if (hit.distance < minDist)
					{
						object = &shapes[i];
						minDist = hit.distance;
					}
				}
				i++;
			}
			int is_shadow = 0;
			if (object != NULL)
			{
				t_raycasthit shadow_hit;
				shadow_ray.origin = add_vec3(hit.point, mul_vec3(hit.normal, shadow_bias));
				shadow_ray.direction = normalize_vec3(sub_vec3(light.position, shadow_ray.origin));
				int j = 0;
				minDist = INFINITY;
				while (j < num_shapes)
				{
					if (intersects_shape(&shadow_ray, &shapes[j], &shadow_hit))
					{
						is_shadow = 1;
						break ;
					}
					j++;
				}
			}
			if (object != NULL)
			{
				hit.light_dir = normalize_vec3(sub_vec3(light.position, hit.point));
				t_vec3 diffuse = mul_vec3(object->color, dot_vec3(hit.normal, hit.light_dir));
				if (is_shadow)
					diffuse = mul_vec3(diffuse, 0.3f);
				put_pixel_mlx_img(env->mlx_img, x, y, ft_get_color(diffuse));
			}
			x++;
		}
		y++;
	}
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	ft_printf("rendered in: %fs\n", cpu_time_used);

	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx_img->img, 0, 0);

	mlx_hook(env->mlx->win_ptr, 2, 0, key_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
