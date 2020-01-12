/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/12 16:21:27 by wkorande         ###   ########.fr       */
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

static int parse_shape(int fd, t_shape_type type, t_shape *shape)
{
	char *line;

	if (type < 0 || !shape)
	{
		ft_printf("type error\n");
	}
	while(ft_get_next_line(fd, &line))
	{
		if (line[0] == '#' || line[0] == '\0')
		{
			free(line);
			ft_printf("type: %d\n", shape->type);
			ft_printf("pos: %.3f, %.3f, %.3f\n", shape->position.x, shape->position.y, shape->position.z);
			ft_printf("rot: %.3f, %.3f, %.3f\n", shape->rotation.x, shape->rotation.y, shape->rotation.z);
			ft_printf("scale: %.3f, %.3f, %.3f\n", shape->scale.x, shape->scale.y, shape->scale.z);
			ft_printf("color: %.3f, %.3f, %.3f\n", shape->color.x, shape->color.y, shape->color.z);
			ft_printf("normal: %.3f, %.3f, %.3f\n", shape->normal.x, shape->normal.y, shape->normal.z);
			ft_printf("radius: %.3f\n", shape->radius);
			ft_printf("shape done.\n\n");
			return (0);
		}
		shape->type = type;
		if (ft_strnequ(line, "pos", 3))
			shape->position = parse_vec3(line + 3);
		if (ft_strnequ(line, "rot", 3))
			shape->rotation = parse_vec3(line + 3);
		if (ft_strnequ(line, "scale", 5))
			shape->scale = parse_vec3(line + 5);
		if (ft_strnequ(line, "col", 3))
			shape->color = parse_vec3(line + 3);
		if (ft_strnequ(line, "normal", 6))
			shape->normal = parse_vec3(line + 6);
		if (ft_strnequ(line, "radius", 6))
			shape->radius = ft_strtod(line + 7);
		free(line);
	}

	return (1);
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
	while(1)
	{
		if (!num_shapes)
		{
			if (ft_get_next_line(fd, &line))
			{
				num_shapes = ft_atoi(line);
				free(line);
			}
			shapes = (t_shape*)malloc(sizeof(t_shape) * num_shapes);
			ft_printf("created %d shapes\n", num_shapes);
			*count = num_shapes;
		}
		if (ft_get_next_line(fd, &line))
		{
			t_shape_type type = parse_shape_type(line);
			ft_printf("shape: %d %s\n", type, line);
			free (line);
			parse_shape(fd, type, &shapes[i]);
			i++;
		}
		else
			break ;
	}
	close(fd);
	return (shapes);
}
