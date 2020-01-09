/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 18:43:48 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/09 18:44:56 by wkorande         ###   ########.fr       */
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

#endif
