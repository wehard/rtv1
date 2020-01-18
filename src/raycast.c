/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 13:31:26 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

int				trace_ray(t_ray *ray, t_scene *scene, t_raycasthit *hit, int ignore_origin_obj)
{
	t_raycasthit	cur_hit;
	double			min_dist;
	int				i;
	int				hit_found;

	min_dist = INFINITY;
	hit_found = 0;
	i = 0;
	while (i < scene->num_objects)
	{
		if (ignore_origin_obj && ray->origin_object == &scene->objects[i])
		{
			i++;
			continue ;
		}
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

static double	calc_light_contrib(t_scene *scene, t_raycasthit *hit)
{
	t_vec3	light_dir;
	double	light_contrib;
	double 	ldn;
	int		i;

	light_contrib = 0.0;
	i = 0;
	while (i < scene->num_lights)
	{
		light_dir = ft_normalize_vec3(ft_sub_vec3(scene->lights[i].position, hit->point));
		ldn = ft_dot_vec3(light_dir, hit->normal);
		light_contrib += ldn * scene->lights[i].intensity;
		i++;
	}
	return (light_contrib);
}

static double	calc_shadow_contrib(t_scene *scene, t_raycasthit *origin)
{
	t_ray			ray;
	t_raycasthit	hit;
	int				i;
	double			shadow;
	double			s_increment;

	s_increment = 1.0 / scene->num_lights;
	shadow = 1.0;
	ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(origin->normal, SHADOW_BIAS));
	ray.origin_object = origin->object;
	i = 0;
	while (i < scene->num_lights)
	{
		ray.direction = ft_normalize_vec3(ft_sub_vec3(scene->lights[i].position, ray.origin));
		if (trace_ray(&ray, scene, &hit, TRUE))
			shadow -= (s_increment * scene->lights[i].intensity);
		i++;
	}
	return (shadow);
}

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth)
{
	int hit_found;

	if (depth > MAX_RAY_DEPTH)
	{
		hit->color = scene->ambient_color;
		return (0);
	}
	hit_found = trace_ray(ray, scene, hit, FALSE);
	if (hit_found)
	{
		hit->color = hit->object->color;

		double light_contribution = ft_clamp_d(calc_light_contrib(scene, hit), 0.0, 1.0);
		double shadow = ft_clamp_d(calc_shadow_contrib(scene, hit), 0.0, 1.0);
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
