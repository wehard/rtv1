/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:47:45 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/15 22:15:03 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <math.h>

t_vec3	make_vec3(double x, double y, double z)
{
	t_vec3 v;

	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

void	set_vec3(t_vec3 *v, double x, double y, double z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

t_vec3	add_vec3(t_vec3 v1, t_vec3 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return (v1);
}

t_vec3	sub_vec3(t_vec3 v1, t_vec3 v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return (v1);
}

t_vec3	mul_vec3(t_vec3 v, double t)
{
	t_vec3 new;

	new.x = v.x * t;
	new.y = v.y * t;
	new.z = v.z * t;
	return (new);
}

t_vec3	mul_vec3_vec3(t_vec3 v1, t_vec3 v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
	return (v1);
}

t_vec3	div_vec3(t_vec3 v, double t)
{
	t_vec3 new;

	if (t == 0.0)
		return (v);
	new.x = v.x / t;
	new.y = v.y / t;
	new.z = v.z / t;
	return (new);
}

double	len_vec3(t_vec3 v)
{
	double l;

	if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f)
		return (0.0f);
	l = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return (l);
}


double	dot_vec3(t_vec3 v1, t_vec3 v2)
{
	double dot;

	dot = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return (dot);
}

t_vec3	normalize_vec3(t_vec3 v)
{
	t_vec3 new;
	double l;

	l = len_vec3(v);
	new = div_vec3(v, l);
	return (new);
}

t_vec3	invert_vec3(t_vec3 v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return (v);
}

t_vec3	reflect_vec3(t_vec3 v, t_vec3 n)
{
	t_vec3 r;

	n = normalize_vec3(n);
	r =  sub_vec3(v, mul_vec3(n, 2.0f * dot_vec3(v, n)));
	return (r);
}
