/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/12 16:19:55 by wkorande         ###   ########.fr       */
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
	hit->distance = 0.0f;
	if (shape->type == SPHERE)
		hit_found = intersects_sphere(ray, shape, hit);
	if (shape->type == PLANE)
		hit_found = intersects_plane(ray, shape, hit);
	if (hit_found)
	{
		hit->point = point_on_ray(ray, hit->t);
		hit->distance = len_vec3(sub_vec3(hit->point, ray->origin));
		hit->normal = hit_normal(hit->point, shape);
		hit->hit_shape = shape;
	}
	return (hit_found);
}

static void print_shape_info(t_shape *shape)
{
	ft_printf("type: %d\n", shape->type);
	ft_printf("pos: %.3f, %.3f, %.3f\n", shape->position.x, shape->position.y, shape->position.z);
	ft_printf("rot: %.3f, %.3f, %.3f\n", shape->rotation.x, shape->rotation.y, shape->rotation.z);
	ft_printf("scale: %.3f, %.3f, %.3f\n", shape->scale.x, shape->scale.y, shape->scale.z);
	ft_printf("color: %.3f, %.3f, %.3f\n", shape->color.x, shape->color.y, shape->color.z);
	ft_printf("radius: %.3f\n\n", shape->radius);
}

int	main(void)
{
	t_env *env;
	clock_t start, end;
	double cpu_time_used;
	env = init_env(1000, 1000, "RTv1");

	int num_shapes;
	t_shape *shapes;

	shapes = read_scene("scenes/test.rt", &num_shapes);
	for (int i = 0; i < num_shapes; i++)
	{
		print_shape_info(&shapes[i]);

	}
	ft_printf("----\n");
	/*
	shapes[0].type = PLANE;
	shapes[0].position = make_vec3(0.0, -2.0, 5.0);
	shapes[0].color = make_vec3(0.7f, 0.6f, 0.3f);
	shapes[0].normal = make_vec3(0.0f, 1.0f, 0.0f);

	shapes[1].type = SPHERE;
	shapes[1].position = make_vec3(1.5, 0.0, 20.0);
	shapes[1].radius = 2.0f;
	shapes[1].color = make_vec3(1.0f, 0.0f, 0.0f);

	shapes[2].type = SPHERE;
	shapes[2].position = make_vec3(-1.5, 0.0, 20.0);
	shapes[2].radius = 2.0f;
	shapes[2].color = make_vec3(0.0f, 1.0f, 0.0f);

	shapes[3].type = SPHERE;
	shapes[3].position = make_vec3(5.0, -1.0, 20.0);
	shapes[3].radius = 1.0f;
	shapes[3].color = make_vec3(0.0f, 0.0f, 1.0f);

	shapes[4].type = SPHERE;
	shapes[4].position = make_vec3(0.0, 80.0, 200.0);
	shapes[4].radius = 80.0f;
	shapes[4].color = make_vec3(0.3f, 0.3f, 0.3f);
	*/
	t_light light;
	light.position = make_vec3(0.0f, 100.0f, 17.0f);
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

	mlx_string_put(env->mlx->mlx_ptr, env->mlx->win_ptr, 100, 100, 0xFFFFFF, "rendering");
	clear_mlx_img(env->mlx_img);
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

			float min_dist = INFINITY;
			t_shape *closest_shape = NULL;
			t_raycasthit closest_hit;
			t_vec3	closest_normal;
			t_raycasthit hit;
			//hit.hit_shape = NULL;
			int i = 0;
			while (i < num_shapes)
			{
				if (intersects_shape(&ray, &shapes[i], &hit))
				{
					if (hit.distance < min_dist)
					{
						closest_shape = &shapes[i];
						closest_normal = hit_normal(hit.point, closest_shape);
						closest_hit = hit;
						min_dist = hit.distance;
					}
				}
				i++;
			}
			int is_shadow = 0;
			if (closest_shape != NULL)
			{
				if (x == 450 && y == 500)
				{
					ft_printf("closest color: %.3f, %.3f, %.3f\n", closest_shape->color.x, closest_shape->color.y, closest_shape->color.z );
				}
				t_raycasthit shadow_hit;
				shadow_ray.origin = add_vec3(closest_hit.point, mul_vec3(closest_normal, shadow_bias));
				shadow_ray.direction = normalize_vec3(sub_vec3(light.position, shadow_ray.origin));
				shadow_ray.origin_shape = closest_shape;
				int j = 0;
				min_dist = INFINITY;
				while (j < num_shapes)
				{
					if (shadow_ray.origin_shape == &shapes[j])
					{
						j++;
						continue;
					}
					if (intersects_shape(&shadow_ray, &shapes[j], &shadow_hit))
					{
						is_shadow = 1;
						break ;
					}
					j++;
				}
			}
			if (closest_shape != NULL)
			{
				t_vec3 light_dir = normalize_vec3(sub_vec3(light.position, closest_hit.point));
				t_vec3 diffuse = mul_vec3(closest_shape->color, dot_vec3(closest_normal, light_dir));
				if (is_shadow)
					diffuse = mul_vec3(diffuse, 0.4f);
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

	mlx_hook(env->mlx->win_ptr, 2, (1L<<0), key_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
