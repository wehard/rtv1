/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:48 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 12:18:46 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

#include "rtv1.h"
#include "vector.h"

typedef struct	s_ray
{
	t_vec3		origin;
	t_vec3		direction;
}				t_ray;

typedef struct s_raycasthit
{
	//t_ray		*ray;
	t_vec3		point;
	t_vec3		normal;
	//t_shape		*shape;
	t_vec3		light_dir;
	float		t;
	float		distance;
}				t_raycasthit;

t_ray			make_ray(t_vec3 o, t_vec3 d);
t_vec3			point_on_ray(t_ray *r, float t);

#endif
