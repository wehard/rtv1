/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/24 00:06:17 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/10 10:21:57 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <mlx.h>
#include "rtv1.h"
#include "libft.h"

t_mlx_img	*create_mlx_image(t_env *env, int width, int height)
{
	t_mlx_img *img;

	if (!(img = (t_mlx_img*)malloc(sizeof(t_mlx_img))))
	{
		ft_putendl_fd("error: create_mlx_image failed!", 2);
		return (NULL);
	}
	img->width = width;
	img->height = height;
	if (!(img->img = mlx_new_image(env->mlx->mlx_ptr,
									img->width, img->height)))
	{
		ft_putendl_fd("error: mlx_new_image failed!", 2);
		return (NULL);
	}
	if (!(img->d_addr = mlx_get_data_addr(img->img, &img->bpp,
										&img->size_line, &img->endian)))
	{
		ft_putendl_fd("error: mlx_get_data_addr failed!", 2);
		return (NULL);
	}
	img->bpp /= 8;
	return (img);
}

void		clear_mlx_img(t_mlx_img *img)
{
	if (!img || !img->d_addr)
		return ;
	ft_memset(img->d_addr, 0, img->width * img->height * img->bpp);
}

void		put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c)
{
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	*(int*)(img->d_addr + (((y * img->width) + x) * img->bpp)) = c;
}
