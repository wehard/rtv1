/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 20:37:20 by wkorande          #+#    #+#             */
/*   Updated: 2020/02/05 15:18:38 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

#ifndef __linux__

# define KEY_ESC		53
# define KEY_NUM_MULT	67
# define KEY_NUM_DIV	75
# define KEY_NUM_PLUS	69
# define KEY_NUM_MINUS	78
# define KEY_NUM_ENTER	76
# define KEY_NUM_0		82
# define KEY_NUM_1		83
# define KEY_NUM_2		84
# define KEY_NUM_3		85
# define KEY_NUM_4		86
# define KEY_NUM_5		87
# define KEY_NUM_6		88
# define KEY_NUM_7		89
# define KEY_NUM_8		91
# define KEY_NUM_9		92
# define KEY_NUM_LEFT	86
# define KEY_NUM_RIGHT	88
# define KEY_NUM_UP		91
# define KEY_NUM_DOWN	84
# define KEY_UP			126
# define KEY_DOWN		125
# define KEY_LEFT		123
# define KEY_RIGHT		124
# define KEY_SPACE		49
# define KEY_1			18
# define KEY_2			19
# define KEY_3			20
# define KEY_L			37
# define KEY_P			35

#else

# define KEY_ESC		65307
# define KEY_SPACE		32
# define KEY_UP			65362
# define KEY_DOWN		65364
# define KEY_LEFT		65361
# define KEY_RIGHT		65363

#endif

#endif
