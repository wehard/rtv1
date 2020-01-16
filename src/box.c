/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 17:57:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 15:13:05 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"

t_shape	make_box()
{
	t_shape s;
	s.type = BOX;
	return (s);
}

t_vec3 calculate_hit_normal_box(t_ray *ray, t_raycasthit *hit)
{
	t_vec3 normal;
	t_vec3 near = point_on_ray(ray, hit->t);
	t_vec3 far = point_on_ray(ray, hit->t2);
	double eps = 0.001;
	if (ft_abs_d(hit->t - near.x) < eps)
		normal = ft_make_vec3(-1,0,0);
	if (ft_abs_d(hit->t - far.x) < eps)
		normal = ft_make_vec3(1,0,0);
	if (ft_abs_d(hit->t - near.y) < eps)
		normal = ft_make_vec3(0,-1,0);
	if (ft_abs_d(hit->t - far.y) < eps)
		normal = ft_make_vec3(0,1,0);
	if (ft_abs_d(hit->t - near.z) < eps)
		normal = ft_make_vec3(0,0,-1);
	if (ft_abs_d(hit->t - far.z) < eps)
		normal = ft_make_vec3(0,0,1);
	return (normal);
}

int		intersects_box(t_ray *ray, t_shape *box, t_raycasthit *hit)
{
	t_vec3 min = ft_add_vec3(box->position, ft_make_vec3(0,0,0));
	t_vec3 max = ft_add_vec3(box->position, ft_make_vec3(1,1,1));

	double tmin = (min.x - ray->origin.x) / ray->direction.x;
	double tmax = (max.x - ray->origin.x) / ray->direction.x;

	if (tmin > tmax)
		ft_swap_d(&tmin, &tmax);

	double tymin = (min.y - ray->origin.y) / ray->direction.y;
	double tymax = (max.y - ray->origin.y) / ray->direction.y;

	if (tymin > tymax)
		ft_swap_d(&tymin, &tymax);

	if (tmin > tymax || tymin > tmax)
		return (0);

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	double tzmin = (min.z - ray->origin.z) / ray->direction.z;
	double tzmax = (max.z - ray->origin.z) / ray->direction.z;

	if (tzmin > tzmax)
		ft_swap_d(&tzmin, &tzmax);

	if (tmin > tzmax || tzmin > tmax)
		return (0);

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	hit->t = tmin;
	hit->t2 = tmax;
	//hit->normal = calculate_hit_normal_box(ray, hit);
	return (1);
}



