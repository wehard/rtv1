/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 18:37:58 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "vector.h"

t_ray	make_ray(t_vec3 o, t_vec3 d)
{
	t_ray r;

	r.origin = o;
	r.direction = d;
	return (r);
}

t_vec3	point_on_ray(t_ray *r, float t)
{
	t_vec3 p;

	p = mul_vec3(normalize_vec3(add_vec3(r->origin, r->direction)), t);
	return (p);
}
