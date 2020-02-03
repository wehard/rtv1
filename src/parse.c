/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 13:08:03 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/03 13:58:59 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "ft_printf.h"
#include "ft_get_next_line.h"
#include <stdio.h>

t_vec3 parse_vec3(char *str)
{
	t_vec3 v;
	char *token;

	str = ft_strstr(str, " ");
	token = ft_strtok(str, " ");
	v.x = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.y = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	v.z = ft_strtod(token);
	return (v);
}

t_rgba parse_rgba(char *line)
{
	t_rgba c;
	char *token;

	line = ft_strstr(line, " ");
	token = ft_strtok(line, " ");
	c.r = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	c.g = ft_strtod(token);
	token = ft_strtok(NULL, " ");
	c.b = ft_strtod(token);
	c.a = 1.0;
	return (c);
}

int	parse_light(int fd, t_light *light)
{
	char *line;

	if (!light)
		ft_printf("error: light is null!\n");
	light->type = 0;
	light->position = ft_make_vec3(0, 0, 0);
	light->direction = ft_make_vec3(0, 0, -1);
	light->color = ft_make_rgba(1, 1, 1, 1);
	light->intensity = 1.0;
	while(ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "type", 4))
			light->type = ft_atoi(ft_strstr(line, " "));
		else if (ft_strnequ(line, "dir", 3))
			light->direction = parse_vec3(line);
		else if (ft_strnequ(line, "pos", 3))
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

int parse_object(int fd, t_object_type type, t_object *object)
{
	char *line;

	if (type < 0 || !object)
		ft_printf("error: wrong object type!\n");
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
		//else if (ft_strnequ(line, "nor", 3))
		//	object->normal = parse_vec3(line);
		else if (ft_strnequ(line, "rad", 3))
			object->radius = ft_strtod(line + 4);
		else if (line[0] == '#')
		{
			if (type == CYLINDER)
				rotate_cylinder(object, object->rotation);
			if (type == CONE)
				rotate_cone(object, object->rotation);
			if (type == PLANE)
				rotate_plane(object, object->rotation);
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

int parse_camera(int fd, t_camera *camera)
{
	char *line;
	t_vec3 pos;
	t_vec3 look_at;
	double fov;

	if (!camera)
		ft_printf("error: camera is null!\n");
	while(ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "pos", 3))
			pos = parse_vec3(line);
		else if (ft_strnequ(line, "fov", 3))
			fov = ft_strtod(ft_strstr(line, " "));
		else if (ft_strnequ(line, "look_at", 7))
			look_at = parse_vec3(line);
		else if (line[0] == '#')
		{
			init_camera(camera, pos, look_at, fov, (double)WIN_W / (double)WIN_H);
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}
