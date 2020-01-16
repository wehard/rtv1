/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 18:03:59 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 12:03:32 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"

t_rgba	ft_make_rgba(double r, double g, double b, double a)
{
	t_rgba color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}

t_rgba	ft_mul_rgba(t_rgba c, double t)
{
	c.r *= t;
	c.g *= t;
	c.b *= t;
	c.a *= t;
	return (c);
}

t_rgba	ft_lerp_rgba(t_rgba c1, t_rgba c2, double t)
{
	t_rgba color;

	color.r = ft_lerp_f(c1.r, c2.r, t);
	color.g = ft_lerp_f(c1.g, c2.g, t);
	color.b = ft_lerp_f(c1.b, c2.b, t);
	color.a = ft_lerp_f(c1.a, c2.a, t);
	return (color);
}

int		ft_get_color(t_rgba c)
{
	int r;
	int g;
	int b;

	r = (int)(c.r * 255.0f) << 16;
	g = (int)(c.g * 255.0f) << 8;
	b = (int)(c.b * 255.0f);
	return (r | g | b);
}
