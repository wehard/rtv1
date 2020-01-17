/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/17 21:48:54 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

static double	ft_clamp_d(double d, double min, double max)
{
	if (d < min)
		d = min;
	if (d > max)
		d = max;
	return (d);
}

int	trace_ray(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	t_raycasthit	cur_hit;
	double	min_dist = INFINITY;
	int i;
	int hit_found;

	hit_found = 0;
	i = 0;
	while (i < scene->num_objects)
	{
		if (intersects_object(ray, &scene->objects[i], &cur_hit))
		{
			hit_found = TRUE;
			if (cur_hit.distance < min_dist)
			{
				*hit = cur_hit;
				min_dist = cur_hit.distance;
			}
		}
		i++;
	}
	return (hit_found);
}

double	calculate_light_contribution(t_scene *scene, t_raycasthit *hit)
{
	int i;

	double light_contribution = 0.0;
	i = 0;
	while (i < scene->num_lights)
	{
		t_vec3 light_dir = ft_normalize_vec3(ft_sub_vec3(scene->lights[i].position, hit->point));
		double light_dot_normal = ft_dot_vec3(light_dir, hit->normal);
		light_contribution += light_dot_normal * scene->lights[i].intensity;
		i++;
	}
	return (light_contribution);
}

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth)
{
	int hit_found;

	if (depth > MAX_RAY_DEPTH)
	{
		hit->color = scene->ambient_color;
		return (0);
	}
	hit_found = trace_ray(ray, scene, hit);
	if (hit_found)
	{
		hit->color = hit->object->color;

		double light_contribution = ft_clamp_d(calculate_light_contribution(scene, hit), 0.0, 1.0);
		double shadow = raycast_shadow(scene, hit);
		hit->color = ft_mul_rgba(hit->color, light_contribution);
		hit->color = ft_mul_rgba(hit->color, shadow);
		if (hit->object->reflect > 0)
		{
			t_ray reflect_ray;
			t_raycasthit reflect_hit;
			reflect_ray.origin = ft_add_vec3(hit->point, ft_mul_vec3(hit->normal, REFLECT_BIAS));
			reflect_ray.direction = ft_normalize_vec3(ft_reflect_vec3(ray->direction, hit->normal));
			if (raycast(&reflect_ray, scene, &reflect_hit, depth + 1))
				hit->color = ft_lerp_rgba(hit->color, reflect_hit.color, hit->object->reflect);
		}

		//double ray_dot_normal = ft_dot_vec3(ray->direction, hit->normal);
		//double specular = ft_lerp_f(pow(light_dot_normal, 200), 1, 0.5);
		//hit->color = ft_lerp_rgba(scene->ambient_color, hit->color, ft_max_d(0.0, ray_dot_normal));
		//hit->color = ft_mul_rgba(hit->color, specular);
	}
	else
		hit->color = scene->ambient_color;
	return (hit_found);
}

double	raycast_shadow(t_scene *scene, t_raycasthit *origin)
{
	t_ray			ray;
	t_raycasthit	hit;
	int				i;
	int				l;
	double			shadow_strength;
	double			s_increment;

	s_increment = 1.0 / scene->num_lights;

	shadow_strength = 1.0;
	ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(origin->normal, SHADOW_BIAS));
	l = 0;
	while (l < scene->num_lights)
	{
		i = 0;
		while (i < scene->num_objects)
		{
			ray.direction = ft_normalize_vec3(ft_sub_vec3(scene->lights[l].position, ray.origin));
			if (origin->object != &scene->objects[i] && intersects_object(&ray, &scene->objects[i], &hit))
			{
				shadow_strength -= (s_increment * scene->lights[l].intensity);
				break ;
			}
			i++;
		}
		l++;
	}
	return (shadow_strength);
}


