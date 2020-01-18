/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/18 02:01:05 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "vector.h"
# include "time.h"

# define WIN_W 1200
# define WIN_H 800
# define MAX_RAY_DEPTH 5
# define MAX_DISTANCE 200
# define SHADOW_BIAS 0.001
# define REFLECT_BIAS 0.0001
# define TRUE 1
# define FALSE 0

typedef enum		e_object_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	BOX,
	LIGHT
}					t_object_type;


typedef struct		s_rgba
{
	double			r;
	double			g;
	double			b;
	double			a;
}					t_rgba;

typedef struct		s_light
{
	t_vec3			position;
	double			intensity;
	t_rgba			color;
}					t_light;

typedef struct		s_object
{
	t_object_type	type;
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			scale;
	t_rgba			color;
	double			radius;
	t_vec3			normal;
	double			reflect;
}					t_object;

typedef struct	s_ray
{
	t_vec3		origin;
	t_vec3		direction;
	t_object	*origin_object;
}				t_ray;

typedef struct	s_raycasthit
{
	t_vec3		point;
	t_vec3		normal;
	t_object	*object;
	t_vec3		light_dir;
	double		t;
	double		t2;
	double		distance;
	t_rgba		color;
}				t_raycasthit;

typedef struct	s_scene
{
	char		*path;
	int			fd;
	time_t		mod_time;
	t_object	*objects;
	t_light		*lights;
	int			num_lights;
	int			num_objects;
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

int				intersects_object(t_ray *ray, t_object *object, t_raycasthit *hit);

t_object		make_plane();
t_object		make_sphere();
t_object		make_box();
int				intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit);
int				intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit);
int				intersects_box(t_ray *ray, t_object *box, t_raycasthit *hit);
int				intersects_cylinder(t_ray *ray, t_object *sphere, t_raycasthit *hit);

t_vec3 			calculate_hit_normal_box(t_ray *ray, t_raycasthit *hit);

int				raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit, int depth);
double			raycast_shadow(t_scene *scene, t_raycasthit *origin);
int				trace_ray(t_ray *ray, t_scene *scene, t_raycasthit *hit);

t_ray			make_ray(t_vec3 o, t_vec3 d);
t_vec3			point_on_ray(t_ray *r, double t);
void			init_raycasthit(t_raycasthit *hit);


void			print_object_info(t_object *object);
t_vec3			calculate_hit_normal(t_raycasthit *hit);
int				intersects_object(t_ray *ray, t_object *object, t_raycasthit *hit);
time_t			check_mod_time(char *path);

/*
** colors
*/
t_rgba			ft_make_rgba(double r, double g, double b, double a);
t_rgba			ft_mul_rgba(t_rgba c, double t);
t_rgba			ft_lerp_rgba(t_rgba c1, t_rgba c2, double t);
int				ft_get_color(t_rgba c);



#endif
