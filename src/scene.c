/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/13 17:12:08 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
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
	char *token;
	t_vec3 v;

	token = ft_strtok(line, " ");
	v.x = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.y = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.z = ft_strtod(token);
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
			shape->position = parse_vec3(line + 4);
		else if (ft_strnequ(line, "rot", 3))
			shape->rotation = parse_vec3(line + 4);
		else if (ft_strnequ(line, "sca", 3))
			shape->scale = parse_vec3(line + 4);
		else if (ft_strnequ(line, "col", 3))
			shape->color = parse_vec3(line + 4);
		else if (ft_strnequ(line, "nor", 3))
			shape->normal = parse_vec3(line + 4);
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
t_shape	*read_scene(char *path, int *count)
{
	int fd;
	char *line;
	int		num_shapes;
	int i;
	t_shape *shapes;

	fd = open(path, O_RDWR);
	if (fd < 3)
	{
		ft_printf("error: opening scene file");
		return (0);
	}
	num_shapes = 0;
	i = 0;
	while(ft_get_next_line(fd, &line))
	{
		if (!num_shapes)
		{
			num_shapes = ft_atoi(line);
			free(line);
			shapes = (t_shape*)malloc(sizeof(t_shape) * num_shapes);
			ft_printf("created %d shapes\n", num_shapes);
			*count = num_shapes;
		}
		else
		{
			t_shape_type type = parse_shape_type(line);
			ft_printf("shape: %d %s\n", type, line);
			free (line);
			parse_shape(fd, type, &shapes[i]);
			i++;
		}
	}
	close(fd);
	return (shapes);
}