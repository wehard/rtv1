/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 18:38:10 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

int		trace(t_ray *ray, t_scene *scene, t_raycasthit *hit, int stop_at_first)
{
	t_raycasthit	cur_hit;
	double			min_dist;
	int				i;
	int				hit_found;

	min_dist = INFINITY;
	hit_found = FALSE;
	i = 0;
	while (i < scene->num_objects)
	{
		if (intersects_object(ray, &scene->objects[i], &cur_hit))
		{
			if (stop_at_first)
				return (TRUE);
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

static t_rgba shade(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	double ambient_strength;
	t_rgba color;
	t_rgba diffuse;
	t_rgba specular;
	t_rgba ambient;
	int		i;
	double distance;
	double attenuation;

	ambient_strength = .1;
	ambient = ft_mul_rgba(hit->object->color, ambient_strength);
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
			attenuation = 1.0;

		if (!is_in_shadow(&scene->lights[i], scene, hit))
		{
			diffuse = ft_add_rgba(diffuse, calc_diffuse(&scene->lights[i], hit));
			specular = ft_add_rgba(specular, calc_specular(&scene->lights[i], hit, scene->camera.pos));
			diffuse = ft_mul_rgba(diffuse, attenuation);
			specular = ft_mul_rgba(specular, attenuation);
		}
		i++;
	}
	color = ft_mul_rgba_rgba(hit->object->color, ft_add_rgba(ambient, ft_add_rgba(diffuse, specular)));
	return (color);
}

t_rgba	raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit)
{
	t_rgba color;

	color = scene->ambient_color;
	if (trace(ray, scene, hit, FALSE))
		color = shade(ray, scene, hit);
	return (ft_clamp_rgba(color));
}
