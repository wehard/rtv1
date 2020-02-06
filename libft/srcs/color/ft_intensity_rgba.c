/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intensity_rgba.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 12:40:53 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 17:33:07 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

double	ft_intensity_rgba(t_rgba c)
{
	ft_clamp_rgba(c);
	return ((c.r + c.g + c.b) / 3.0);
}
