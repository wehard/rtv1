/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:29:59 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 14:12:02 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "ft_printf.h"
#include "ft_get_next_line.h"
#include <math.h>

int		init_camera(t_camera *cam, t_vec3 pos, t_vec3 look_at, double fov, double aspect)
{
	t_camera_info	ci;

	cam->pos = pos;
	cam->look_at = look_at;
	cam->fov = fov;
	cam->aspect = aspect;
	ci.world_up = ft_make_vec3(0, 1, 0);
	ci.w = ft_normalize_vec3(ft_sub_vec3(cam->pos, cam->look_at));
	ci.u = ft_normalize_vec3(ft_cross_vec3(ci.world_up, ci.w));
	ci.v = ft_cross_vec3(ci.w, ci.u);
	ci.theta = ft_deg_to_rad(cam->fov);
	ci.half_height = tan(ci.theta / 2.0);
	ci.half_width = cam->aspect * ci.half_height;
	cam->lower_left_corner = ft_sub_vec3(cam->pos, ft_sub_vec3(
		ft_mul_vec3(ci.u, ci.half_width), ft_sub_vec3(
			ft_mul_vec3(ci.v, ci.half_height), ci.w)));
	cam->horizontal = ft_mul_vec3(ci.u, ci.half_width * 2);
	cam->vertical = ft_mul_vec3(ci.v, ci.half_height * 2);
	return (1);
}

t_ray	get_camera_ray(t_camera *camera, double u, double v)
{
	t_ray			ray;

	ray.origin_object = NULL;
	ray.origin = camera->pos;
	ray.direction = ft_normalize_vec3(ft_sub_vec3(
		ft_add_vec3(camera->lower_left_corner, ft_add_vec3(
			ft_mul_vec3(camera->horizontal, u), ft_mul_vec3(
				camera->vertical, -v))), ray.origin));
	return (ray);
}

t_vec2i world_to_screen_point(t_camera *camera, t_vec3 wp)
{
	t_vec3 dir;
	t_vec2i sp;
	double aspect = (double)WIN_W / (double)WIN_H;
	dir = ft_normalize_vec3(ft_sub_vec3(wp, camera->pos));
	t_vec3 ipp = ft_mul_vec3(dir, 1.0);
	sp.x = ((ipp.x + aspect * 0.5) / aspect) * WIN_W;
	sp.y =  WIN_H - ((ipp.y + 0.5) * WIN_H);
	return (sp);
}

int parse_camera(int fd, t_camera *camera)
{
	char *line;
	t_vec3 pos;
	t_vec3 look_at;
	double fov;

	if (!camera)
		ft_printf("error: camera is null!\n");
	while(ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "pos", 3))
			pos = ft_parse_vec3(line);
		else if (ft_strnequ(line, "fov", 3))
			fov = ft_strtod(ft_strstr(line, " "));
		else if (ft_strnequ(line, "look_at", 7))
			look_at = ft_parse_vec3(line);
		else if (line[0] == '#')
		{
			init_camera(camera, pos, look_at, fov,
				(double)WIN_W / (double)WIN_H);
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}
