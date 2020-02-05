/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 17:50:07 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 13:33:24 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "vector.h"
# include "time.h"
# include "matrix.h"
# include "color.h"

# define SQR(value) value * value
# define NUM_THREADS 60
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

typedef enum	e_obj_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	LIGHT,
	CAMERA
}				t_obj_type;

typedef enum	e_lgt_type
{
	DIRECTIONAL,
	POINT
}				t_lgt_type;

typedef struct	s_quadratic
{
	double		a;
	double		b;
	double		c;
}				t_quadratic;

typedef struct	s_light
{
	t_lgt_type	type;
	t_vec3		position;
	t_vec3		direction;
	double		intensity;
	t_rgba		color;
}				t_light;

typedef struct	s_object
{
	t_obj_type	type;
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		scale;
	t_rgba		color;
	double		radius;
	t_vec3		start;
	t_vec3		end;
	t_vec3		normal;
}				t_object;

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
	t_object	*selected_object;
}				t_scene;

typedef struct	s_mlx_img
{
	void		*img;
	char		*d_addr;
	int			bpp;
	int			size_line;
	int			endian;
	int			width;
	int			height;
}				t_mlx_img;

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
	int			thread_index;
}				t_env;

t_env			*init_env(int width, int height, char *title);
void			del_env_exit(t_env *env);

int				read_scene(t_scene *scene, char *path);
int				parse_light(int fd, t_light *light);
int				parse_object(int fd, t_obj_type type, t_object *object);
int				parse_camera(int fd, t_camera *camera);
void			render(t_env *env, t_scene *scene);
int				update(void *param);

int				is_in_shadow(t_light *light, t_scene *scene, t_raycasthit *origin);
t_rgba			calc_diffuse(t_light *light, t_raycasthit *hit);
t_rgba			calc_specular(t_light *light, t_raycasthit *hit, t_vec3 cam);
t_mlx_img		*create_mlx_image(t_env *env, int width, int height);
void			clear_mlx_img(t_mlx_img *img);
void			put_pixel_mlx_img(t_mlx_img *img, int x, int y, int c);

int				init_camera(t_camera *camera, t_vec3 pos, t_vec3 look_at, double fov, double aspect);
t_ray			get_camera_ray(t_camera *camera, double u, double v);
t_vec2i			world_to_screen_point(t_camera *camera, t_vec3 wp);

void			init_object(t_object *object);
int				intersects_object(t_ray *ray, t_object *object, t_raycasthit *hit);
int				intersects_plane(t_ray *ray, t_object *plane, t_raycasthit *hit);
int				intersects_sphere(t_ray *ray, t_object *sphere, t_raycasthit *hit);
int				intersects_cylinder(t_ray *ray, t_object *sphere, t_raycasthit *hit);
int				intersects_cone(t_ray *ray, t_object *cone, t_raycasthit *hit);
int				solve_quadratic(t_quadratic q, double *t1, double *t2);

void			rotate_cylinder(t_object *c, t_vec3 rot);
void			rotate_cone(t_object *c, t_vec3 rot);
void			rotate_plane(t_object *p, t_vec3 rot);

t_rgba			raycast(t_ray *ray, t_scene *scene, t_raycasthit *hit);

int				trace(t_ray *ray, t_scene *scene, t_raycasthit *hit, int stop_at_first);

t_vec3			point_on_ray(t_ray *r, double t);
void			init_raycasthit(t_raycasthit *hit);

void			print_object_info(t_object *object);
t_vec3			calculate_hit_normal(t_raycasthit *hit);

time_t			check_mod_time(char *path);

void			panic(char *message);

void			draw_line(t_mlx *mlx, t_vec3 p0, t_vec3 p1, int color);

int				key_press(int key, void *param);
int				mouse_press(int button, int x, int y, void *param);

void			draw_lights(t_env *env, t_scene *scene);
void			print_vec3(t_vec3 v);

#endif
