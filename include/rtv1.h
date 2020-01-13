/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/13 19:20:13 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "vector.h"

# define WIN_W 1280
# define WIN_H 720
# define MAX_DISTANCE 1000

typedef enum		e_shape_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	BOX
}					t_shape_type;

typedef struct		s_light
{
	t_vec3			position;
	float			intensity;
}					t_light;

typedef struct		s_shape
{
	t_shape_type	type;
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			scale;
	t_vec3			color;
	float			radius;
	t_vec3			normal;
	float			reflect;
}					t_shape;

typedef struct	s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	int			is_shadow_ray;
	float		shadow_bias;
	t_shape		*origin_shape;
}				t_ray;

typedef struct	s_raycasthit
{
	t_vec3		point;
	t_vec3		normal;
	t_shape		*hit_shape;
	t_vec3		light_dir;
	float		t;
	float		t2;
	float		distance;
}				t_raycasthit;

typedef struct	s_scene
{
	t_light		light;
	t_shape		*shapes;
	int			num_shapes;
}				t_scene;

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

t_env			*init_env(int width, int height, char *title);
void			del_env_exit(t_env *env);

t_shape			*read_scene(char *path, int *count);

t_mlx_img		*create_mlx_image(t_env *env, int width, int height);
void			clear_mlx_img(t_mlx_img *img);
void			put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c);

int				intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit);

t_shape			make_sphere();
int				intersects_sphere(t_ray *ray, t_shape *sphere, t_raycasthit *hit);

t_shape			make_plane();

int				ft_get_color(t_vec3 c);
int				intersects_plane(t_ray *ray, t_shape *plane, t_raycasthit *hit);

t_shape			make_box();
int				intersects_box(t_ray *ray, t_shape *box, t_raycasthit *hit);

t_ray			make_ray(t_vec3 o, t_vec3 d);
t_vec3			point_on_ray(t_ray *r, float t);

void			print_shape_info(t_shape *shape);
t_vec3			hit_normal(t_raycasthit *hit, t_shape *shape);
int				intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit);

#endif
