/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 19:00:58 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 19:40:45 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"

int		intersects_cone(t_ray *ray, t_object *cone, t_raycasthit *hit)
{
	double cosa = 45.0 / 2.0;
	t_vec3 co = ft_sub_vec3(ray->origin, cone->position);

	double a = (ft_dot_vec3(ray->direction, cone->normal) * ft_dot_vec3(ray->direction, cone->normal)) - (cosa * cosa);
	double b = 2.0 * (ft_dot_vec3(ray->direction, cone->normal) * ft_dot_vec3(co, cone->normal) - ft_dot_vec3(ray->direction, co)*(cosa*cosa));
	double c = ft_dot_vec3(co, cone->normal) * ft_dot_vec3(co, cone->normal) - ft_dot_vec3(co, co) * (cosa * cosa);
	double det = b * b - 4.0 * a * c;
	if (det < 0)
		return (0);
	double t1 = (-b - det) / (2.0 * a);
	double t2 = (-b + det) / (2.0 * a);
	double t = t1;
	if (t < 0 || (t2 > 0 && t2 < t))
		t = t2;
	if (t < 0)
		return (0);
	t_vec3 cp =  ft_add_vec3(ray->origin, ft_mul_vec3(ft_sub_vec3(ray->direction, cone->position), t));
	double h = ft_dot_vec3(cp, cone->normal);
	if (h < 0 || h > cone->scale.y)
		return (0);
	hit->t = t;
	hit->object = cone;
	hit->normal = ft_normalize_vec3(ft_sub_vec3(ft_mul_vec3(cp, ft_dot_vec3(cone->normal, cp) / ft_dot_vec3(cp, cp)), cone->normal));
	return (1);
}
