/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 09:26:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 18:38:12 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rtv1.h"
#include "vector.h"

t_shape	make_sphere()
{
	t_shape t;
	t.type = SPHERE;
	t.position = make_vec3(0.0f, 0.0f, 0.0f);
	t.radius = 1.0f;
	t.rotation = make_vec3(0.0f, 0.0f, 0.0f);
	t.scale = make_vec3(1.0f, 1.0f, 1.0f);
	t.color = make_vec3(1.0f, 1.0f, 1.0f);
	return  (t);
}

int		intersects_sphere(t_ray *ray, t_shape *sphere, t_raycasthit *hit)
{
	float t0;
	float t1;
	float sradius2;

	sradius2 = sphere->radius * sphere->radius;

	t_vec3 L = sub_vec3(sphere->position, ray->origin);
	float tca = dot_vec3(L, ray->direction);
	float d2 = dot_vec3(L, L) - tca * tca;
	if (d2 > sradius2)
		return (0);
	float thc = sqrtf(sradius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return (0);
	}
	hit->t = t0;
	return (1);
}

