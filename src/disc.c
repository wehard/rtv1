/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 18:09:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 18:27:21 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include <math.h>

int	intersects_disc(t_ray *ray, t_object *disc, t_raycasthit *hit)
{
	if (intersects_plane(ray, disc, hit))
	{
		t_vec3 dir = ft_sub_vec3(disc->position, hit->point);
		double d2 = ft_dot_vec3(dir, dir);
		if (sqrt(d2) <= disc->radius)
		{
			hit->object = disc;
			return (TRUE);
		}
	}
	return (FALSE);
}
