/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 01:15:38 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 02:19:55 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

int	intersects_cylinder(t_ray *ray, t_object *sphere, t_raycasthit *hit)
{
	double t0;
	double t1;

	double a = ray->direction.x * ray->direction.x +
				ray->direction.z * ray->direction.z;
	double b = 2.0 * ray->origin.x * ray->origin.x +
				2.0 * ray->origin.z * ray->origin.z;
	double c = ray->origin.x * ray->origin.x +
				ray->origin.z * ray->origin.z - 1.0;
	double b24ac = b * b - 4 * a * c;
	if (b24ac < 0)
	{
		return (FALSE);
	}
	double sqb24ac = sqrt(b24ac);
	t0 = (-b +sqb24ac) / (2 * a);
	t1 = (-b - sqb24ac) / (2 * a);

	if (t0 > t1)
		ft_swap_d(&t0, &t1);

	float y0 = ray->origin.y + t0 * ray->direction.y;
	float y1 = ray->origin.y + t1 * ray->direction.y;

	if (y0 < -1 && y1 < -1)
		return (FALSE);
	if (y0 >= -1 && y0 <= 1)
	{
		if (t0 < 0)
			return (FALSE);
		hit->t = t0;
		hit->t2 = t1;
		hit->object = sphere;
		return (TRUE);
	}
	return (FALSE);
}
