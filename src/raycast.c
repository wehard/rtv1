/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/01 17:35:05 by wkorande         ###   ########.fr       */
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

static int	is_in_shadow(t_light *light, t_scene *scene, t_raycasthit *origin)
{
	t_ray shadow_ray;
	t_raycasthit hit;

	shadow_ray.origin = ft_add_vec3(origin->point, ft_mul_vec3(ft_normalize_vec3(origin->normal), SHADOW_BIAS));
	shadow_ray.origin_object = origin->object;
	if (light->type == POINT)
		shadow_ray.direction = ft_normalize_vec3(ft_sub_vec3(light->position, shadow_ray.origin));
	else
		shadow_ray.direction = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	if (trace(&shadow_ray, scene, &hit))
	{
		if (hit.object != shadow_ray.origin_object)
			return(1);
	}
	return (0);
}

static t_rgba	calc_diffuse(t_light *light, t_raycasthit *hit)
{
	// N hit normal
	// L vec from hit point to light source
	// all vectors normalized!
	t_rgba diffuse;
	t_vec3 light_dir;
	double ndotl;
	double distance;
	double attenuation;


	if (light->type == POINT)
		light_dir = ft_normalize_vec3(ft_sub_vec3(light->position, hit->point));
	else
		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	ndotl = ft_dot_vec3(hit->normal, light_dir);
	diffuse = ft_mul_rgba(light->color, ft_max_d(ndotl, 0.0));
	return (ft_mul_rgba(diffuse, light->intensity));
}

static t_rgba	calc_specular(t_light *light, t_raycasthit *hit, t_vec3 cam)
{
	t_rgba specular;
	t_vec3 light_dir;
	t_vec3 r;
	t_vec3 c;
	double strength = 0.5;
	unsigned int k;

	if (light->type == POINT)
		light_dir = ft_normalize_vec3(ft_sub_vec3(light->position, hit->point));
	else
		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	light_dir = ft_mul_vec3(light_dir, -1);
	r = ft_normalize_vec3(ft_reflect_vec3(light_dir, hit->normal));
	c = ft_normalize_vec3(ft_sub_vec3(cam, hit->point));
	k = 32;
	specular = ft_mul_rgba(ft_mul_rgba(ft_mul_rgba(ft_make_rgba(1,1,1,1), light->intensity), pow(ft_max_d(ft_dot_vec3(r, c), 0.0), k)), strength);
	return (specular);
}

static t_rgba shade(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	double ambient_strength;
	t_rgba color;
	t_rgba diffuse;
	t_rgba specular;
	//t_rgba ambient;
	int		i;
	double distance;
	double attenuation;

	ambient_strength = 1;
	//ambient = ft_mul_rgba(hit->object->color, ambient_strength);
	diffuse = ft_make_rgba(0,0,0,1);
	specular = ft_make_rgba(0,0,0,1);
	i = 0;
	while (i < scene->num_lights)
	{
		if (scene->lights[i].type == POINT)
		{
			distance = ft_len_vec3(ft_sub_vec3(scene->lights[i].position, hit->point));
			attenuation = 1.0 / (1.0 + 0.045 * distance + 0.0075 * SQR(distance));
		}
			attenuation = 1.0; // / 1.0 - (hit->distance / MAX_DISTANCE);

		diffuse = ft_add_rgba(diffuse, calc_diffuse(&scene->lights[i], hit));
		specular = ft_add_rgba(specular, calc_specular(&scene->lights[i], hit, scene->camera.pos));

		diffuse = ft_mul_rgba(diffuse, attenuation);
		specular = ft_mul_rgba(specular, attenuation);
		i++;
	}
	color = ft_mul_rgba_rgba(hit->object->color, ft_add_rgba(diffuse, specular));
	return (color);
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
	return (ft_clamp_rgba(color));
}
