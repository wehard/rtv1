/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 09:26:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 19:16:33 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rtv1.h"
#include "libft.h"
#include "vector.h"
#include "ft_printf.h"

t_object	make_sphere()
{
	t_object t;
	t.type = SPHERE;
	t.position = ft_make_vec3(0.0, 0.0, 0.0);
	t.radius = 1.0;
	t.rotation = ft_make_vec3(0.0, 0.0, 0.0);
	t.scale = ft_make_vec3(1.0, 1.0, 1.0);
	t.color = ft_make_rgba(1.0, 1.0, 1.0, 1.0);
	return (t);
}


int		intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit)
{
	t_vec3 oc = ft_sub_vec3(ray->origin, sphere->position);
	double a = ft_dot_vec3(ray->direction, ray->direction);
	double b = 2.0 * ft_dot_vec3(oc, ray->direction);
	double c = ft_dot_vec3(oc, oc) - sphere->radius * sphere->radius;
	double disc = b * b - 4 * a * c;
	if (disc < 0)
		return (FALSE);
	else
	{
		hit->t = (-b - sqrt(disc)) / (2.0 * a);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, sphere->position));
	}
	return (TRUE);
}
