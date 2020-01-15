/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/15 15:56:26 by wkorande         ###   ########.fr       */
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

static t_shape_type parse_shape_type(char *line)
{
	if (ft_strequ(line, "PLANE"))
		return PLANE;
	if (ft_strequ(line, "SPHERE"))
		return SPHERE;
	if (ft_strequ(line, "CYLINDER"))
		return CYLINDER;
	if (ft_strequ(line, "CONE"))
		return CONE;
	if (ft_strequ(line, "BOX"))
		return BOX;
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
	return (v);
}

static void init_shape(t_shape *shape)
{
	shape->type = 0;
	shape->position = make_vec3(0.0, 0.0, 0.0);
	shape->rotation = make_vec3(0.0, 0.0, 0.0);
	shape->scale = make_vec3(0.0, 0.0, 0.0);
	shape->normal = make_vec3(0.0, 0.0, 0.0);
	shape->color = make_vec3(0.0, 0.0, 0.0);
	shape->radius = 0.0;
	shape->reflect = 0.0;
}

static int parse_shape(int fd, t_shape_type type, t_shape *shape)
{
	char *line;

	if (type < 0 || !shape)
	{
		ft_printf("type error\n");
	}
	init_shape(shape);
	while(ft_get_next_line(fd, &line))
	{
		shape->type = type;
		if (ft_strnequ(line, "pos", 3))
			shape->position = parse_vec3(line);
		else if (ft_strnequ(line, "rot", 3))
			shape->rotation = parse_vec3(line);
		else if (ft_strnequ(line, "sca", 3))
			shape->scale = parse_vec3(line);
		else if (ft_strnequ(line, "col", 3))
			shape->color = parse_vec3(line);
		else if (ft_strnequ(line, "nor", 3))
			shape->normal = parse_vec3(line);
		else if (ft_strnequ(line, "rad", 3))
			shape->radius = ft_strtod(line + 4);
		else if (ft_strnequ(line, "ref", 3))
			shape->reflect = ft_strtod(line + 4);
		else if (line[0] == '#')
		{
			free(line);
			print_shape_info(shape);
			ft_printf("shape done.\n\n");
			return (1);
		}
		free(line);
	}
	return (0);
}

/*
**	Reads a scene from file and sets count to number of shapes
*/
int		read_scene(t_scene *scene, char *path)
{
	int fd;
	char *line;
	int i;

	scene->path = path;
	fd = open(path, O_RDWR);
	if (!scene)
		ft_putendl("null");
	scene->mod_time = check_mod_time(scene->path);
	if (fd < 3)
	{
		ft_printf("error: opening scene file");
		return (0);
	}
	scene->num_shapes = 0;
	i = 0;
	while(ft_get_next_line(fd, &line))
	{
		if (!scene->num_shapes)
		{
			scene->num_shapes = ft_atoi(line);
			free(line);
			scene->shapes = (t_shape*)malloc(sizeof(t_shape) * scene->num_shapes);
			ft_printf("created %d shapes\n", scene->num_shapes);
		}
		else if (ft_strnequ(line, "LIGHT", 5))
			scene->light.position = parse_vec3(line);
		else if (ft_strnequ(line, "COLOR", 5))
			scene->ambient_color = parse_vec3(line);
		else
		{
			t_shape_type type = parse_shape_type(line);
			ft_printf("shape: %d %s\n", type, line);
			free (line);
			parse_shape(fd, type, &(scene->shapes)[i]);
			i++;
		}
	}
	close(fd);
	return (1);
}

time_t	check_mod_time(char *path)
{
	struct stat stat_info;

	stat(path, &stat_info);
	//ft_printf("mod time: %d\n", stat_info.st_mtime);
	return (stat_info.st_mtime);
}
