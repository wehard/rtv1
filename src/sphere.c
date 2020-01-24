/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 09:26:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/24 17:02:22 by wkorande         ###   ########.fr       */
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

static t_vec3	sphere_normal(t_ray *ray, t_object *sphere, double t)
{
	t_vec3 n;

	n = ft_add_vec3(ray->origin, ft_mul_vec3(ray->direction, t));
	n = ft_normalize_vec3(ft_sub_vec3(n, sphere->position));
	if (ft_dot_vec3(ray->direction, n) > 0.0001)
		n = ft_mul_vec3(n, -1);
	return (n);
}

/*
int		intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit)
{
	double a;
	double b;
	double c;
	double d;

	a = ft_dot_vec3(ray->direction, ray->direction);
	b = 2.0 * ft_dot_vec3(ray->direction, ft_sub_vec3(ray->origin, sphere->position));
	c = ft_dot_vec3(ft_sub_vec3(ray->origin, sphere->position),
		ft_sub_vec3(ray->origin, sphere->position)) - sphere->radius * sphere->radius;
	d = b * b - 4 * a * c;
	if (d > 0)
	{
		hit->t = (-b - sqrt(d)) / a;
		if (hit->t < MAX_DISTANCE && hit->t > MIN_DISTANCE)
		{
			hit->point = point_on_ray(ray, hit->t);
			hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, sphere->position));
			return (TRUE);
		}
		hit->t = (-b + sqrt(d)) / a;
		if (hit->t < MAX_DISTANCE && hit->t > MIN_DISTANCE)
		{
			hit->point = point_on_ray(ray, hit->t);
			hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, sphere->position));
			return (TRUE);
		}
	}
	return (FALSE);
}
*/

int		intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit)
{
	t_vec3 oc = ft_sub_vec3(ray->origin, sphere->position);
	double a = ft_dot_vec3(ray->direction, ray->direction);
	double b = 2.0 * ft_dot_vec3(oc, ray->direction);
	double c = ft_dot_vec3(oc, oc) - sphere->radius * sphere->radius;
	double disc = b*b - 4*a*c;
	if (disc < 0)
		return (FALSE);
	else
	{
		hit->t = (-b - sqrt(disc)) / (2.0*a);
		hit->point = point_on_ray(ray, hit->t);
		hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, sphere->position));
	}
	return (TRUE);
}

/*
int		intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit)
{
	double t0;
	double t1;
	double sradius2;

	sradius2 = sphere->radius * sphere->radius;

	t_vec3 L = ft_sub_vec3(sphere->position, ray->origin);
	double tca = ft_dot_vec3(L, ray->direction);
	double d2 = ft_dot_vec3(L, L) - tca * tca;
	if (d2 > sradius2)
		return (0);
	double thc = sqrt(sradius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1)
		ft_swap_d(&t0, &t1);
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return (0);
	}
	hit->t = t0;
	hit->point = point_on_ray(ray, hit->t);
	hit->normal = ft_normalize_vec3(ft_sub_vec3(hit->point, sphere->position));
	return (1);
}
*/

