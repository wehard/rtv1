/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 13:24:31 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/24 13:50:11 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>
/*
**	Up vector is set to 0, 1, 0
*/
t_camera	init_camera(t_vec3 pos, t_vec3 look_at, double fov, double aspect)
{
	t_camera c;

	c.pos = pos;
	c.look_at = look_at;
	c.fov = fov;
	c.aspect = aspect;
	c.theta = ft_deg_to_rad(fov);
	c.h_height = tan(c.theta / 2.0);
	c.h_width = aspect * c.h_height;
	c.w = ft_normalize_vec3(ft_sub_vec3(c.pos, c.look_at));
	c.u = ft_normalize_vec3(ft_cross_vec3(ft_make_vec3(0, 1, 0), c.w));
	c.v = ft_cross_vec3(c.w, c.u);
	c.llc = ft_sub_vec3(c.pos, ft_sub_vec3(ft_sub_vec3(ft_mul_vec3(c.u, c.h_width), ft_mul_vec3(c.v, c.h_height)), c.w));
	c.horizontal = ft_mul_vec3(c.u, c.h_width * 2.0);
	c.vertical = ft_mul_vec3(c.v, c.h_height * 2.0);
	return (c);
}

t_ray	get_ray(t_camera *cam, double s, double t)
{
	t_ray r;

	r.origin = cam->pos;
	r.direction = ft_sub_vec3(ft_add_vec3(cam->llc, ft_add_vec3(ft_mul_vec3(cam->horizontal, s), ft_mul_vec3(cam->vertical, t))), cam->pos);
	return (r);
}
