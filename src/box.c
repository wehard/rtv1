/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 17:57:25 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/23 15:45:27 by wkorande         ###   ########.fr       */
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

int		intersects_box(t_ray *ray, t_object *box, t_raycasthit *hit)
{

	if (intersects_plane_xy(ray, box, box->scale.z, hit))
		return (TRUE);
	if (intersects_plane_xz(ray, box, box->scale.y, hit))
		return (TRUE);
	if (intersects_plane_yz(ray, box, box->scale.x, hit))
		return (TRUE);

	if (intersects_plane_xy(ray, box, -box->scale.z, hit))
		return (TRUE);
	if (intersects_plane_xz(ray, box, -box->scale.y, hit))
		return (TRUE);
	if (intersects_plane_yz(ray, box, -box->scale.x, hit))
		return (TRUE);
	return (FALSE);
}



