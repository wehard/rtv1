/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_rgba.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 17:06:12 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 17:07:48 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

t_rgba	ft_make_rgba(double r, double g, double b, double a)
{
	t_rgba color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}
