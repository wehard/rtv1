/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:12:08 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 12:53:39 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdlib.h>
#include "mlx.h"
#include "color.h"
#include "vector.h"
#include "libft.h"
#include "ft_get_next_line.h"
#include "ft_printf.h"
#include <math.h>

void		init_light(t_light *light)
{
	light->type = 0;
	light->position = ft_make_vec3(0, 0, 0);
	light->rotation = ft_make_vec3(0, 0, 0);
	light->direction = ft_make_vec3(0, -1, 0);
	light->color = ft_make_rgba(1, 1, 1, 1);
	light->intensity = 1.0;
}

void		rotate_light(t_light *light, t_vec3 rot)
{
	t_vec3 res;
	t_vec3 v;

	v = ft_make_vec3(0, -1, 0);
	res = ft_rotate_vec3(v, rot);
	light->direction = ft_normalize_vec3(res);
}

int			is_in_shadow(t_light *light, t_scene *scene, t_hit *origin)
{
	t_ray	shadow_ray;
	t_hit	hit;

	shadow_ray.origin = ft_add_vec3(origin->point,
		ft_mul_vec3(ft_normalize_vec3(origin->normal), SHADOW_BIAS));
	shadow_ray.origin_object = origin->object;
	if (light->type == POINT)
		shadow_ray.direction =
			ft_normalize_vec3(ft_sub_vec3(light->position, shadow_ray.origin));
	else
		shadow_ray.direction =
			ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	if (trace(&shadow_ray, scene, &hit, TRUE) &&
		hit.object != shadow_ray.origin_object)
		return (TRUE);
	return (FALSE);
}

t_rgba		calc_diffuse(t_light *light, t_hit *hit)
{
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

t_rgba		calc_specular(t_light *light, t_hit *hit, t_vec3 cam)
{
	t_rgba			specular;
	t_vec3			light_dir;
	t_vec3			r;
	t_vec3			c;
	unsigned int	k;

	if (light->type == POINT)
		light_dir = ft_normalize_vec3(ft_sub_vec3(light->position, hit->point));
	else
		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	light_dir = ft_mul_vec3(light_dir, -1);
	r = ft_normalize_vec3(ft_reflect_vec3(light_dir, hit->normal));
	c = ft_normalize_vec3(ft_sub_vec3(cam, hit->point));
	k = 32;
	specular = ft_mul_rgba(ft_mul_rgba(light->color, light->intensity),
		pow(ft_max_d(ft_dot_vec3(r, c), 0.0), k));
	return (specular);
}
