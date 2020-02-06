/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 21:49:19 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 15:43:49 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "libft.h"
#include "ft_get_next_line.h"
#include "ft_printf.h"

static void	verify_camera(t_camera *c)
{
	double len;

	len = ft_len_vec3(ft_sub_vec3(c->pos, c->look_at));
	if (len < 0.0001)
		panic("check camera position and look_at!");
}

int			parse_camera(int fd, t_camera *camera)
{
	char	*line;
	t_vec3	pos;
	t_vec3	look_at;
	double	fov;

	if (!camera)
		ft_printf("error: camera is null!\n");
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "pos", 3))
			pos = ft_parse_vec3(line);
		else if (ft_strnequ(line, "fov", 3))
			fov = ft_strtod(ft_strstr(line, " "));
		else if (ft_strnequ(line, "look_at", 7))
			look_at = ft_parse_vec3(line);
		else if (line[0] == '#')
		{
			init_camera(camera, pos, look_at, fov);
			verify_camera(camera);
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

int			parse_light(int fd, t_light *light)
{
	char *line;

	init_light(light);
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnequ(line, "type", 4))
			light->type = ft_atoi(ft_strstr(line, " "));
		else if (ft_strnequ(line, "pos", 3))
			light->position = ft_parse_vec3(line);
		else if (ft_strnequ(line, "rot", 3))
			light->rotation = ft_parse_vec3(line);
		else if (ft_strnequ(line, "col", 3))
			light->color = ft_parse_rgba(line);
		else if (ft_strnequ(line, "int", 3))
			light->intensity = ft_strtod(line + 4);
		else if (line[0] == '#')
		{
			free(line);
			rotate_light(light, light->rotation);
			return (1);
		}
		free(line);
	}
	return (0);
}

static void	set_object_property(char *line, t_object *object)
{
	if (ft_strnequ(line, "pos", 3))
		object->position = ft_parse_vec3(line);
	else if (ft_strnequ(line, "rot", 3))
		object->rotation = ft_parse_vec3(line);
	else if (ft_strnequ(line, "sca", 3))
		object->scale = ft_parse_vec3(line);
	else if (ft_strnequ(line, "col", 3))
		object->color = ft_parse_rgba(line);
	else if (ft_strnequ(line, "rad", 3))
		object->radius = ft_strtod(line + 4);
}

int			parse_object(int fd, t_obj_type type, t_object *object)
{
	char *line;

	if (type < 0 || !object)
		panic("wrong object type!");
	init_object(object);
	while (ft_get_next_line(fd, &line))
	{
		object->type = type;
		if (line[0] == '#')
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
		else
			set_object_property(line, object);
		free(line);
	}
	return (0);
}
