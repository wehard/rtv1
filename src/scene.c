/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 12:46:06 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/11 12:56:02 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "ft_printf.h"

/*
**	Reads a scene from file and sets count to number of shapes
*/
int	read_scene(char *path, int *shapes, int *count)
{
	int fd;

	fd = open(path, O_RDWR);
	if (fd < 3)
	{
		ft_printf("error: opening scene file");
		return (0);
	}
}
