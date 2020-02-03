/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 14:00:52 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "vector.h"
# include "time.h"
# include "matrix.h"

# define SQR(value) value * value
# define WIN_W 800
# define WIN_H 600
# define MAX_RAY_DEPTH 5
# define MAX_DISTANCE 1000
# define MIN_DISTANCE 0.1
# define SHADOW_BIAS 0.001
# define REFLECT_BIAS 0.001
# define RAYS_PER_PIXEL	1
# define TRUE 1
# define FALSE 0

typedef enum		e_object_type
{
	PLANE,
	DISC,
	SPHERE,
	CYLINDER,
	CONE,
	BOX,
	LIGHT,
	CAMERA
}					t_object_type;

typedef enum		e_light_type
{
	DIRECTIONAL,
	POINT
}					t_light_type;

typedef struct	s_quadratic
{
	double		a;
	double		b;
	double		c;
}				t_quadratic;

typedef struct		s_rgba
{
	double			r;
	double			g;
	double			b;
	double			a;
}					t_rgba;

typedef struct		s_light
{
	t_light_type	type;
	t_vec3			position;
	t_vec3			direction;
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
	t_vec3			start;
	t_vec3			end;
	t_vec3			normal;
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


typedef struct	s_camera_info
{
	t_vec3		world_up;
	t_vec3		u;
	t_vec3		v;
	t_vec3		w;
	double		theta;
	double		half_height;
	double		half_width;
}				t_camera_info;

typedef struct	s_camera
{
	t_vec3		pos;
	t_vec3		look_at;
	double		fov;
	double		scale;
	double		aspect;
	t_vec3		horizontal;
	t_vec3		vertical;
	t_vec3		lower_left_corner;
}				t_camera;

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
	t_camera	camera;
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
t_vec3			parse_vec3(char *str);
t_rgba			parse_rgba(char *line);
int				parse_light(int fd, t_light *light);
int				parse_object(int fd, t_object_type type, t_object *object);
int				parse_camera(int fd, t_camera *camera);
void 			render(t_env *env, t_scene *scene);
int				update(void *param);

t_mlx_img		*create_mlx_image(t_env *env, int width, int height);
void			clear_mlx_img(t_mlx_img *img);
void			put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c);

int				init_camera(t_camera *camera, t_vec3 pos, t_vec3 look_at, double fov, double aspect);
t_ray 			get_camera_ray(t_camera *camera, double u, double v);

void 			init_object(t_object *object);
int				intersects_object(t_ray *ray, t_object *object, t_raycasthit *hit);
int				intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit);
int				intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit);
int				intersects_cylinder(t_ray *ray, t_object *sphere, t_raycasthit *hit);
int				intersects_disc(t_ray *ray, t_object *disc, t_raycasthit *hit);
int				intersects_cone(t_ray *ray, t_object *cone, t_raycasthit *hit);
int 			solve_quadratic(t_quadratic q, double *t1, double *t2);

void			rotate_cylinder(t_object *c, t_vec3 rot);
void			rotate_cone(t_object *c, t_vec3 rot);
void			rotate_plane(t_object *p, t_vec3 rot);

t_rgba			raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit);
//int				trace(t_ray *ray, t_scene *scene, t_raycasthit *hit);

t_vec3			point_on_ray(t_ray *r, double t);
void			init_raycasthit(t_raycasthit *hit);

void			print_object_info(t_object *object);
t_vec3			calculate_hit_normal(t_raycasthit *hit);

time_t			check_mod_time(char *path);

/*
** colors
*/
t_rgba			ft_make_rgba(double r, double g, double b, double a);
t_rgba			ft_mul_rgba(t_rgba c, double t);
t_rgba 			ft_add_rgba(t_rgba c1, t_rgba c2);
t_rgba 			ft_sub_rgba(t_rgba c1, t_rgba c2);
t_rgba			ft_lerp_rgba(t_rgba c1, t_rgba c2, double t);
t_rgba			ft_clamp_rgba(t_rgba c);
t_rgba			ft_mul_rgba_rgba(t_rgba a, t_rgba b);
int				ft_get_color(t_rgba c);

void			draw_line(t_mlx *mlx, t_vec3 p0, t_vec3 p1, int color);

#endif
