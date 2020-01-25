/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 20:21:19 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "rtv1.h"
#include "ft_printf.h"
#include "ft_get_next_line.h"
#include <stdio.h>

static t_object_type parse_object_type(char *line)
{
	if (ft_strequ(line, "PLANE"))
		return PLANE;
	if (ft_strequ(line, "DISC"))
		return DISC;
	if (ft_strequ(line, "SPHERE"))
		return SPHERE;
	if (ft_strequ(line, "CYLINDER"))
		return CYLINDER;
	if (ft_strequ(line, "CONE"))
		return CONE;
	if (ft_strequ(line, "BOX"))
		return BOX;
	if (ft_strequ(line, "LIGHT"))
		return LIGHT;
	return (-1);
}

static t_vec3 parse_vec3(char *line)
{
	char **split;
	t_vec3 v;

	split = ft_strsplit(line, ' ');
	v.x = ft_strtod(split[1]);
	v.y = ft_strtod(split[2]);
	v.z = ft_strtod(split[3]);
	free(split[0]);
	free(split[1]);
	free(split[2]);
	free(split[3]);
	free(split);
	return (v);
}

static t_rgba parse_rgba(char *line)
{
	char **split;
	t_rgba v;

	split = ft_strsplit(line, ' ');

	v.r = ft_strtod(split[1]);
	v.g = ft_strtod(split[2]);
	v.b = ft_strtod(split[3]);
	if (!split[4])
		v.a = 1.0;
	else
		v.a = ft_strtod(split[4]);
	free(split[0]);
	free(split[1]);
	free(split[2]);
	free(split[3]);
	if (!split[4])
		free(split[4]);
	free(split);
	return (v);
}
static void init_object(t_object *object)
{
	object->type = 0;
	object->position = ft_make_vec3(0.0, 0.0, 0.0);
	object->rotation = ft_make_vec3(0.0, 0.0, 0.0);
	object->scale = ft_make_vec3(0.0, 0.0, 0.0);
	object->radius = 0.0;
	object->normal = ft_make_vec3(0.0, 0.0, 0.0);
	object->color = ft_make_rgba(0.0, 0.0, 0.0, 1.0);
	object->reflect = 0.0;
	object->start = ft_make_vec3(0, 0, 0);
	object->end = ft_make_vec3(1, 1, 1);
}

static int	parse_light(int fd, t_object_type type, t_light *light)
{
	char *line;

	if (type != LIGHT || !light)
		ft_printf("error: parsing light object!\n");
	light->position = ft_make_vec3(0,0,0);
	light->color = ft_make_rgba(1,1,1,1);
	light->intensity = 1.0;
	while(ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "pos", 3))
			light->position = parse_vec3(line);
		else if (ft_strnequ(line, "col", 3))
			light->color = parse_rgba(line);
		else if (ft_strnequ(line, "int", 3))
			light->intensity = ft_strtod(line + 4);
		else if (line[0] == '#')
		{
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

static int parse_object(int fd, t_object_type type, t_object *object)
{
	char *line;

	if (type < 0 || !object)
		ft_printf("object type error\n");
	init_object(object);
	while(ft_get_next_line(fd, &line))
	{
		object->type = type;
		if (ft_strnequ(line, "pos", 3))
			object->position = parse_vec3(line);
		else if (ft_strnequ(line, "rot", 3))
			object->rotation = parse_vec3(line);
		else if (ft_strnequ(line, "sca", 3))
			object->scale = parse_vec3(line);
		else if (ft_strnequ(line, "col", 3))
			object->color = parse_rgba(line);
		else if (ft_strnequ(line, "nor", 3))
			object->normal = parse_vec3(line);
		else if (ft_strnequ(line, "rad", 3))
			object->radius = ft_strtod(line + 4);
		else if (ft_strnequ(line, "ref", 3))
			object->reflect = ft_strtod(line + 4);
		else if (ft_strnequ(line, "start", 5))
			object->start = parse_vec3(line);
		else if (ft_strnequ(line, "end", 3))
			object->end = parse_vec3(line);
		else if (line[0] == '#')
		{
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

/*
**	Reads a scene from file
*/
int		read_scene(t_scene *scene, char *path)
{
	int fd;
	char *line;
	int obj_index;
	int light_index;

	scene->path = path;
	fd = open(path, O_RDWR);
	if (!scene)
		ft_putendl("null");
	scene->mod_time = check_mod_time(scene->path);
	if (fd < 3)
	{
		ft_printf("error: opening scene file\n");
		return (0);
	}
	scene->num_objects = 0;
	scene->num_lights = 0;
	obj_index = 0;
	light_index = 0;
	while(ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "OBJECTS", 6) && !scene->num_objects)
		{
			scene->num_objects = ft_atoi(line + 7);
			scene->objects = (t_object*)malloc(sizeof(t_object) * scene->num_objects);
		}
		else if (ft_strnequ(line, "LIGHTS", 5) && !scene->num_lights)
		{
			scene->num_lights = ft_atoi(line + 6);
			scene->lights = (t_light*)malloc(sizeof(t_light) * scene->num_lights);
			ft_printf("lights: %d\n", scene->num_lights);
		}
		else if (ft_strnequ(line, "COLOR", 5))
			scene->ambient_color = parse_rgba(line);
		else if (ft_strnequ(line, "FOV", 3))
			scene->options.fov = ft_strtod(line + 3);
		else if (ft_strnequ(line, "CAM", 3))
			scene->camera.pos = parse_vec3(line);
		else if (ft_strnequ(line, "LOOKAT", 6))
			scene->camera.look_at = parse_vec3(line);
		else
		{
			t_object_type type = parse_object_type(line);
			if (type == LIGHT)
				parse_light(fd, type, &(scene->lights)[light_index++]);
			else
				parse_object(fd, type, &(scene->objects)[obj_index++]);
		}
		free(line);
	}
	close(fd);
	return (1);
}

time_t	check_mod_time(char *path)
{
	struct stat stat_info;

	stat(path, &stat_info);
	return (stat_info.st_mtime);
}
