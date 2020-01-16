/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 20:49:56 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

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

int	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth)
{
	int 			hit_found;

	if (depth > MAX_RAY_DEPTH)
	{
		hit->color = scene->ambient_color;
		return (0);
	}
	hit_found = trace_ray(ray, scene, hit);
	if (hit_found)
	{
		hit->color = hit->object->color;
		if (depth == 0 && raycast_shadow(scene, hit))
			hit->color = ft_mul_rgba(hit->color, 0.3);
		if (hit->object->reflect > 0)
		{
			t_ray reflect_ray;
			t_raycasthit reflect_hit;
			reflect_ray.origin = ft_add_vec3(hit->point, ft_mul_vec3(hit->normal, REFLECT_BIAS));
			reflect_ray.direction = ft_normalize_vec3(ft_reflect_vec3(ray->direction, hit->normal));
			if (raycast(&reflect_ray, scene, &reflect_hit, depth + 1))
				hit->color = ft_lerp_rgba(hit->color, reflect_hit.color, hit->object->reflect);
		}
		//t_vec3 l_dir = ft_normalize_vec3(ft_sub_vec3(scene->light.position, hit->point));
		//double light_dot_normal = ft_dot_vec3(l_dir, hit->normal);
		//double ray_dot_normal = ft_dot_vec3(ray->direction, hit->normal);
		//double specular = ft_lerp_f(pow(light_dot_normal, 200), 1, 0.5);
		//hit->color = ft_lerp_rgba(scene->ambient_color, hit->color, ft_max_d(0.0, ray_dot_normal));
		//hit->color = ft_lerp_rgba(hit->color, hit->color, ft_max_d(0.0, light_dot_normal));
		//hit->color = ft_mul_rgba(hit->color, specular);
	}
	else
		hit->color = scene->ambient_color;
	return (hit_found);
}

int	raycast_shadow(t_scene *scene, t_raycasthit *origin)
{
	t_ray			ray;
	t_raycasthit	hit;
	int				i;
	int				l;

	ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(origin->normal, SHADOW_BIAS));
	i = 0;
	while (i < scene->num_objects)
	{
		l = 0;
		while (l < scene->num_lights)
		{
			ray.direction = ft_normalize_vec3(ft_sub_vec3(scene->lights[l].position, ray.origin));
			if (/*origin->object != &scene->objects[i] && */intersects_object(&ray, &scene->objects[i], &hit))
				return (1);
			l++;
		}
		i++;
	}
	return (0);
}


