/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:12:08 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 19:13:34 by wkorande         ###   ########.fr       */
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

static void	init_light(t_light *light)
{
	light->type = 0;
	light->position = ft_make_vec3(0, 0, 0);
	light->direction = ft_make_vec3(0, 0, -1);
	light->color = ft_make_rgba(1, 1, 1, 1);
	light->intensity = 1.0;
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

int			parse_light(int fd, t_light *light)
{
	char *line;

	init_light(light);
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "type", 4))
			light->type = ft_atoi(ft_strstr(line, " "));
		else if (ft_strnequ(line, "dir", 3))
			light->direction = ft_parse_vec3(line);
		else if (ft_strnequ(line, "pos", 3))
			light->position = ft_parse_vec3(line);
		else if (ft_strnequ(line, "col", 3))
			light->color = ft_parse_rgba(line);
		else if (ft_strnequ(line, "int", 3))
			light->intensity = ft_strtod(line + 4);
		else if (line[0] == '#')
		{
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}
