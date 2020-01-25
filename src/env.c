/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/30 17:30:56 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/25 20:26:14 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdlib.h>
#include <mlx.h>

t_env		*init_env(int width, int height, char *title)
{
	t_env	*env;

	if (!(env = (t_env*)malloc(sizeof(t_env))))
		return (NULL);
	if (!(env->mlx = (t_mlx*)malloc(sizeof(t_mlx))))
		return (NULL);
	env->width = width;
	env->height = height;
	env->mlx->mlx_ptr = mlx_init();
	env->mlx->win_ptr = mlx_new_window(env->mlx->mlx_ptr, width, height, title);
	env->mlx_img = create_mlx_image(env, width, height);
	env->scene = (t_scene*)malloc(sizeof(t_scene));
	return (env);
}

void		del_env_exit(t_env *env)
{
	free(env->scene->lights);
	free(env->scene->objects);
	free(env->scene);
	mlx_destroy_image(env->mlx->mlx_ptr, env->mlx_img->img);
	mlx_destroy_window(env->mlx->mlx_ptr, env->mlx->win_ptr);
	free(env->mlx_img);
	free(env->mlx);
	free(env);
	exit(EXIT_SUCCESS);
}
