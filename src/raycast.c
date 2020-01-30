/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/30 17:23:22 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

static int		trace(t_ray *ray, t_scene *scene, t_raycasthit *hit)
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

static double	calc_light(t_light *light, t_raycasthit *hit)
{
	t_vec3 dir;
	double dist;
	double amount;
	double ldn;
	double spec;

	amount = 0.0;
	dist = ft_len_vec3(ft_sub_vec3(light->position, hit->point));
	if (light->type == POINT)
		dir = ft_normalize_vec3(ft_sub_vec3(light->position, hit->point));
	else
		dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	ldn = ft_dot_vec3(dir, hit->normal);
	spec = ft_lerp_f(pow(ldn, 200), 1, 0.5);
	amount += ldn * light->intensity * spec;
	return (amount);
}

static double	calc_shadow(t_light *light, t_scene *scene, t_raycasthit *origin)
{
	t_ray shadow_ray;
	t_raycasthit hit;
	double amount;

	amount = 0.0;
	shadow_ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(ft_normalize_vec3(origin->normal), SHADOW_BIAS));
	shadow_ray.origin_object = origin->object;
	if (light->type == POINT)
		shadow_ray.direction = ft_normalize_vec3(ft_sub_vec3(light->position, shadow_ray.origin));
	else
		shadow_ray.direction = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	if (trace(&shadow_ray, scene, &hit))
		if (hit.object != shadow_ray.origin_object)
			amount += light->intensity;
	return (amount);
}

static t_rgba	calc_reflect(t_scene *scene, t_vec3 point, t_vec3 idir, t_vec3 normal, int depth)
{
	t_ray			reflect_ray;
	t_raycasthit	reflect_hit;
	t_rgba			color;

	reflect_ray.origin = ft_add_vec3(point, ft_mul_vec3(normal, REFLECT_BIAS));
	reflect_ray.direction = ft_normalize_vec3(ft_reflect_vec3(idir, normal));
	color = raycast(&reflect_ray, scene, &reflect_hit, depth + 1);
	//hit->color = ft_lerp_rgba(hit->color, reflect_hit.color, hit->object->reflect);
	return (color);
}

static t_rgba shade(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	double	amount;
	int		i;
	t_rgba	reflect_color;

	hit->color = hit->object->color;
	amount = 0.0;
	i = 0;
	while (i < scene->num_lights)
	{
		amount += calc_light(&scene->lights[i], hit) * 1.0 - (hit->distance / MAX_DISTANCE);
		amount -= (calc_shadow(&scene->lights[i], scene, hit) / scene->num_lights) * 0.3;
		i++;
	}
	hit->color = ft_mul_rgba(hit->color, ft_clamp_d(amount, 0, 1));
	if (hit->object->reflect > 0)
	{
		reflect_color = calc_reflect(scene, hit->point, ray->direction, hit->normal, 0);
		hit->color = ft_lerp_rgba(hit->color, reflect_color, hit->object->reflect);
	}
	return (hit->color);
}

t_rgba	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth)
{
	t_rgba color;
	if (depth > MAX_RAY_DEPTH)
	{
		color = scene->ambient_color;
		return (color);
	}
	color = scene->ambient_color;
	if (trace(ray, scene, hit))
	{
		color = shade(ray, scene, hit);
	}
	return (color);
}
