/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:49:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 08:19:08 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"
#include <stdio.h>
#include "ray.h"
#include "keys.h"
#include <math.h>

int		ft_get_color(t_vec3 c)
{
	int r;
	int g;
	int b;

	r = (int)(c.x * 255.0f) << 16;
	g = (int)(c.y * 255.0f) << 8;
	b = (int)(c.z * 255.0f);
	return (r | g | b);
}

int		key_press(int key, void *param)
{
	t_env *env;

	env = (t_env*)param;
	if (key == KEY_ESC)
		del_env_exit(env);
	return (0);
}

int		intersects_sphere(t_ray *ray, t_shape *sphere, float *t)
{
	float t0;
	float t1;
	float sradius2;

	sradius2 = sphere->radius * sphere->radius;

	t_vec3 L = sub_vec3(sphere->position, ray->origin);
	float tca = dot_vec3(L, ray->direction);
	float d2 = dot_vec3(L, L) - tca * tca;
	if (d2 > sradius2)
		return (0);
	float thc = sqrtf(sradius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return (0);
	}
	*t = t0;
	return (1);
}

int	main(void)
{
	t_env *env;

	env = init_env(1280, 720, "RTv1");

	t_vec3 v1;

	v1 = make_vec3(1.0, 0.0, 0.0);
	v1 = mul_vec3(v1, 4.0);
	v1 = div_vec3(v1, 2.0);
	ft_printf("%-15s %f\n", "len:", len_vec3(v1));
	ft_printf("%-15s %f\n", "len normalized:", len_vec3(normalize_vec3(v1)));

	t_shape shapes[3];

	shapes[0].type = SPHERE;
	shapes[0].position = make_vec3(0.0, 3.0, 20.0);
	shapes[0].radius = 3.0f;
	shapes[0].color = make_vec3(1.0f, 0.0f, 0.0f);

	shapes[1].type = SPHERE;
	shapes[1].position = make_vec3(0.0, 0.0, 20.0);
	shapes[1].radius = 3.5f;
	shapes[1].color = make_vec3(0.0f, 1.0f, 0.0f);

	shapes[2].type = SPHERE;
	shapes[2].position = make_vec3(5.0, 0.0, 20.0);
	shapes[2].radius = 1.0f;
	shapes[2].color = make_vec3(0.0f, 0.0f, 1.0f);

	t_light light;
	light.position = make_vec3(-10.0f, 20.0f, 20.0f);
	light.intensity = 1.0f;

	t_ray ray;
	ray.origin = make_vec3(0.0, 0.0, 0.0);
	ray.direction = make_vec3(0.0, 0.0, 1.0);

	t_ray shadow_ray;

	float t = -100.0f;
	float fov = 45.0f;
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
			float minDist = 1000.0f;
			int i = 0;
			t_shape *object = NULL;
			while (i < 3)
			{
				if (intersects_sphere(&ray, &shapes[i], &t))
				{
					float distance = len_vec3(sub_vec3(point_on_ray(&ray, t), ray.origin));
					if (distance < minDist)
					{
						object = &shapes[i];
						minDist = distance;
					}
				}
				i++;
			}
			int is_shadow = 0;
			if (object != NULL)
			{
				t_vec3 por = point_on_ray(&ray, t);
				shadow_ray.origin = add_vec3(por, mul_vec3(sub_vec3(por, object->position), 0.001f));
				shadow_ray.direction = normalize_vec3(sub_vec3(light.position, shadow_ray.origin));

				float t2 = -1000.0f;
				int j = 0;
				while (j < 3)
				{
					if (intersects_sphere(&shadow_ray, &shapes[j], &t2))
					{
						is_shadow = 1;
						break ;
					}
					j++;
				}
			}
			if (object != NULL)
			{
				if (!is_shadow)
					mlx_pixel_put(env->mlx->mlx_ptr, env->mlx->win_ptr, x, y, ft_get_color(object->color));
				else
					mlx_pixel_put(env->mlx->mlx_ptr, env->mlx->win_ptr, x, y, ft_get_color(make_vec3(object->color.x*0.2f, object->color.y*0.2f,object->color.z*0.2f)));
			}
			x++;
		}
		y++;
	}

	mlx_hook(env->mlx->win_ptr, 2, 0, key_press, (void*)env);
	mlx_loop(env->mlx->mlx_ptr);
	return (0);
}
