/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:48 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/09 23:07:38 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

#include "vector.h"

typedef struct	s_ray
{
	t_vec3		origin;
	t_vec3		direction;
}				t_ray;

t_ray			make_ray(t_vec3 o, t_vec3 d);
t_vec3			point_on_ray(t_ray *r, float t);

#endif
