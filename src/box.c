/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 17:57:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 17:45:25 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"

t_object	make_box()
{
	t_object s;
	s.type = BOX;
	return (s);
}

t_vec3 calculate_hit_normal_box(t_object *box, t_raycasthit *hit)
{
	t_vec3 normal;
	t_vec3 box_min = ft_sub_vec3(box->position, ft_div_vec3(box->scale, 2));
	t_vec3 box_max = ft_add_vec3(box->position, ft_div_vec3(box->scale, 2));
	double eps = 0.01;
	if (ft_abs_d(hit->point.x - box_min.x) < eps)
		normal = ft_make_vec3(-1, 0, 0);
	if (ft_abs_d(hit->point.x - box_max.x) < eps)
		normal = ft_make_vec3(1, 0 ,0);
	if (ft_abs_d(hit->point.y - box_min.y) < eps)
		normal = ft_make_vec3(0, -1, 0);
	if (ft_abs_d(hit->point.y - box_max.y) < eps)
		normal = ft_make_vec3(0, 1, 0);
	if (ft_abs_d(hit->point.z - box_min.z) < eps)
		normal = ft_make_vec3(0, 0, -1);
	if (ft_abs_d(hit->point.z - box_max.z) < eps)
		normal = ft_make_vec3(0, 0, 1);
	return (normal);
}

int		intersects_box(t_ray *ray, t_object *box, t_raycasthit *hit)
{
	t_vec3 box_min = ft_sub_vec3(box->position, ft_div_vec3(box->scale, 2));
	t_vec3 box_max = ft_add_vec3(box->position, ft_div_vec3(box->scale, 2));
	double t1 = 0.0;
	double t2 = 0.0;
	double tnear = -1000.0;
	double tfar = 1000.0;
	if (ray->direction.x == 0)
	{
		if (ray->origin.x < box_min.x || ray->origin.x > box_min.x)
			return (0);
	}
	else
	{
		t1 = (box_min.x - ray->origin.x) / ray->direction.x;
		t2 = (box_max.x - ray->origin.x) / ray->direction.x;
		if (t1 > t2)
			ft_swap_d(&t1, &t2);
		if (t1 > tnear)
			tnear = t1;
		if (t2 < tfar)
			tfar = t2;
		if (tnear > tfar)
			return (0);
		if (tfar < 0)
			return (0);
	}
	hit->t = tnear;
	hit->t2 = tfar;
	return (1);
}



