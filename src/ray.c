/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/09 18:51:17 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "vector.h"

t_ray	make_ray(t_vec3 o, t_vec3 d)
{
	t_ray r;

	r.origin = o;
	r.direction = d;
	return (r);
}

t_vec3	point_at_ray(t_ray *r, float t)
{
	t_vec3 p;

	p = mul_vec3(add_vec3(r->origin, r->direction), t);
	return (p);
}
