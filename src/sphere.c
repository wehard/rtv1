/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 09:26:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 18:44:30 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rtv1.h"
#include "libft.h"
#include "vector.h"

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
	return (1);
}


