/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/06 12:56:49 by wkorande         ###   ########.fr       */
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

static t_obj_type	parse_object_type(char *line)
{
	if (ft_strequ(line, "PLANE"))
		return (PLANE);
	if (ft_strequ(line, "SPHERE"))
		return (SPHERE);
	if (ft_strequ(line, "CYLINDER"))
		return (CYLINDER);
	if (ft_strequ(line, "CONE"))
		return (CONE);
	if (ft_strequ(line, "LIGHT"))
		return (LIGHT);
	if (ft_strequ(line, "CAMERA"))
		return (CAMERA);
	return (-1);
}

static void			init_scene(t_scene *scene, char *path, time_t modtime)
{
	scene->ambient_color = RGBA_BLACK;
	scene->num_lights = 0;
	scene->lights = NULL;
	scene->num_objects = 0;
	scene->objects = NULL;
	scene->path = path;
	scene->selected_object = NULL;
	scene->mod_time = modtime;
	scene->l_index = 0;
	scene->o_index = 0;
	scene->selected_light = NULL;
}

static void			parse_scene_header(char *line, t_scene *scene)
{
	if (ft_strnequ(line, "OBJECTS", 6) && !scene->num_objects)
	{
		scene->num_objects = ft_atoi(line + 7);
		scene->objects =
			(t_object*)malloc(sizeof(t_object) * scene->num_objects);
	}
	else if (ft_strnequ(line, "LIGHTS", 5) && !scene->num_lights)
	{
		scene->num_lights = ft_atoi(line + 6);
		scene->lights = (t_light*)malloc(sizeof(t_light) * scene->num_lights);
	}
}

int					read_scene(t_scene *sc, char *path)
{
	int			fd;
	char		*line;
	t_obj_type	type;

	if ((fd = open(path, O_RDWR)) < 3)
		panic("error opening scene file");
	init_scene(sc, path, check_mod_time(sc->path));
	while (ft_get_next_line(fd, &line))
	{
		if (!sc->num_objects || !sc->num_lights)
			parse_scene_header(line, sc);
		else if (ft_strnequ(line, "COLOR", 5))
			sc->ambient_color = ft_parse_rgba(line);
		else
		{
			type = parse_object_type(line);
			if (type == CAMERA || type == LIGHT)
				type == CAMERA ? (parse_camera(fd, &sc->camera)) :
					parse_light(fd, &(sc->lights)[sc->l_index++]);
			else if (type >= 0 && type < 4)
				parse_object(fd, type, &(sc->objects)[sc->o_index++]);
		}
		free(line);
	}
	close(fd);
	return (sc->num_lights == sc->l_index && sc->num_objects == sc->o_index);
}

time_t				check_mod_time(char *path)
{
	struct stat stat_info;

	stat(path, &stat_info);
	return (stat_info.st_mtime);
}
