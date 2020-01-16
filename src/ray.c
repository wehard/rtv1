/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 15:56:52 by wkorande         ###   ########.fr       */
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

t_vec3	point_on_ray(t_ray *r, double t)
{
	t_vec3 p;

	p = ft_mul_vec3(ft_normalize_vec3(ft_add_vec3(r->origin, r->direction)), t);
	return (p);
}

void	init_raycasthit(t_raycasthit *hit)
{
	hit->point = ft_make_vec3(0, 0, 0);
	hit->normal = ft_make_vec3(0, 0, 0);
	hit->shape = NULL;
	hit->t = 0.0;
	hit->t2 = 0.0;
	hit->distance = 0.0;
	hit->color = ft_make_rgba(0.0, 0.0, 0.0, 1.0);
}
