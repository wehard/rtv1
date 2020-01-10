/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/10 13:11:48 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

#include "vector.h"

typedef enum 		e_shape_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CONE
}					t_shape_type;

typedef struct		s_shape
{
	t_shape_type	type;
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			scale;
	t_vec3			color;
	float			radius;
}					t_shape;

typedef struct		s_mlx_img
{
	void			*img;
	char			*d_addr;
	int				bpp;
	int				size_line;
	int				endian;
	int				width;
	int				height;
}					t_mlx_img;

typedef struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
}				t_mlx;

typedef struct	s_env
{
	t_mlx		*mlx;
	t_mlx_img	*mlx_img;
	int			width;
	int			height;
}				t_env;

t_env		*init_env(int width, int height, char *title);
void		del_env_exit(t_env *env);

t_mlx_img	*create_mlx_image(t_env *env, int width, int height);
void		clear_mlx_img(t_mlx_img *img);
void		put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c);

#endif
