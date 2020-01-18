/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 18:08:58 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

int	intersects_cylinder(t_ray *ray, t_object *cyl, t_raycasthit *hit)
{
	double a;

	a = ft_dot_vec3(ray->direction, cyl->normal);
	if (a < cyl->radius)
	{
		hit->object = cyl;
		return (1);
	}
	return (0);
}
