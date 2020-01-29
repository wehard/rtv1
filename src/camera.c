/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:29:59 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/29 18:20:11 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "ft_printf.h"
#include <math.h>

int	init_camera(t_camera *camera, t_vec3 pos, t_vec3 look_at, double fov, double aspect)
{
	camera->pos = pos;
	camera->look_at = look_at;
	camera->fov = fov;
	camera->aspect = aspect;
	ft_printf("camera:\n");
	ft_printf("%8s %.3f, %.3f, %.3f\n", "pos:", pos.x, pos.y, pos.z);
	ft_printf("%8s %.3f, %.3f, %.3f\n", "look_at:", look_at.x, look_at.y, look_at.z);
	ft_printf("%8s %.3f\n", "fov:", fov);
	return (1);
}

//return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
//t_ray get_camera_ray(t_camera *camera, int x, int y)
t_ray get_camera_ray(t_camera *camera, double u, double v)
{
	t_camera_info	ci;
	t_ray			ray;

	ray.origin_object = NULL;
	ci.world_up = ft_make_vec3(0, 1, 0);
	ci.w = ft_normalize_vec3(ft_sub_vec3(camera->pos, camera->look_at));
	ci.u = ft_normalize_vec3(ft_cross_vec3(ci.world_up, ci.w));
	ci.v = ft_cross_vec3(ci.w, ci.u);
	ci.theta = ft_deg_to_rad(camera->fov);
	ci.half_height = tan(ci.theta / 2.0);
	ci.half_width = camera->aspect * ci.half_height;
	camera->lower_left_corner = ft_sub_vec3(camera->pos, ft_sub_vec3(ft_mul_vec3(ci.u, ci.half_width), ft_sub_vec3(ft_mul_vec3(ci.v, ci.half_height), ci.w)));
	camera->horizontal = ft_mul_vec3(ci.u, ci.half_width * 2);
	camera->vertical = ft_mul_vec3(ci.v, ci.half_height * 2);
	ray.origin = camera->pos;
	ray.direction = ft_normalize_vec3(ft_sub_vec3(ft_add_vec3(camera->lower_left_corner, ft_add_vec3(ft_mul_vec3(camera->horizontal, u), ft_mul_vec3(camera->vertical, 1.0 - v - 1.0))), ray.origin));
	return (ray);
}
