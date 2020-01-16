/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 15:09:57 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "vector.h"
# include "time.h"

# define WIN_W 1280
# define WIN_H 720
# define MAX_RAY_DEPTH 5
# define MAX_DISTANCE 200
# define SHADOW_BIAS 0.01
# define TRUE 1
# define FALSE 0

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
	double			intensity;
	t_vec3			color;
}					t_light;

typedef struct		s_rgba
{
	double			r;
	double			g;
	double			b;
	double			a;
}					t_rgba;

typedef struct		s_shape
{
	t_shape_type	type;
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			scale;
	t_rgba			color;
	double			radius;
	t_vec3			normal;
	double			reflect;
}					t_shape;

typedef struct	s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	t_shape		*origin_shape;
}				t_ray;

typedef struct	s_raycasthit
{
	t_vec3		point;
	t_vec3		normal;
	t_shape		*shape;
	t_vec3		light_dir;
	double		t;
	double		t2;
	double		distance;
	t_rgba		hit_color;
}				t_raycasthit;

typedef struct	s_scene
{
	char		*path;
	int			fd;
	time_t		mod_time;
	t_light		light;
	t_shape		*shapes;
	int			num_shapes;
	t_rgba		ambient_color;
	double		fov;
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
	t_scene		*scene;
	int			width;
	int			height;
}				t_env;

t_env			*init_env(int width, int height, char *title);
void			del_env_exit(t_env *env);

int				read_scene(t_scene *scene, char *path);
void 			render(t_env *env, t_scene *scene);
int				update(void *param);

t_mlx_img		*create_mlx_image(t_env *env, int width, int height);
void			clear_mlx_img(t_mlx_img *img);
void			put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c);

int				intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit);

t_shape			make_sphere();
int				intersects_sphere(t_ray *ray, t_shape *sphere, t_raycasthit *hit);

t_shape			make_plane();

int				intersects_plane(t_ray *ray, t_shape *plane, t_raycasthit *hit);

t_shape			make_box();
int				intersects_box(t_ray *ray, t_shape *box, t_raycasthit *hit);
t_vec3 			calculate_hit_normal_box(t_ray *ray, t_raycasthit *hit);

t_ray			make_ray(t_vec3 o, t_vec3 d);
t_vec3			point_on_ray(t_ray *r, double t);

void			print_shape_info(t_shape *shape);
t_vec3			calculate_hit_normal(t_raycasthit *hit);
int				intersects_shape(t_ray *ray, t_shape *shape, t_raycasthit *hit);
time_t			check_mod_time(char *path);

/*
** colors
*/
t_rgba			ft_make_rgba(double r, double g, double b, double a);
t_rgba			ft_mul_rgba(t_rgba c, double t);
t_rgba			ft_lerp_rgba(t_rgba c1, t_rgba c2, double t);
int				ft_get_color(t_rgba c);

#endif
