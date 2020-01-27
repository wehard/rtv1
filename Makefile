# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/05 13:58:13 by wkorande          #+#    #+#              #
#    Updated: 2020/01/27 13:17:28 by wkorande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

CFLAGS = -Wall -Wextra -Werror

SRC = rtv1.c\
	env.c\
	scene.c\
	mlx_image.c\
	ray.c\
	sphere.c\
	plane.c\
	cylinder.c\
	cone.c\
	disc.c\
	color.c\
	object.c\
	raycast.c\
	draw_line.c\
	camera.c\
	parse.c

SRCDIR = src

SRCS = $(addprefix $(SRCDIR)/, $(SRC))

OBJS = $(SRC:.c=.o)

INCL = include

LIBFT = ../libft

FLAGS = #-Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	make -C $(LIBFT)
	gcc $(FLAGS) -o $(NAME) -I$(INCL) -I $(LIBFT)/includes $(SRCS) -L$(LIBFT) -lft -lmlx -lm -pthread -framework OpenGL -framework AppKit

debug:
	gcc -g $(FLAGS) -o $(NAME) -I$(INCL) -I $(LIBFT)/includes $(SRCS) -L$(LIBFT) -lft -lmlx -lm -pthread -framework OpenGL -framework AppKit

linux:
	make -C $(LIBFT)
	gcc -g $(FLAGS) -o $(NAME) -I$(INCL) -I $(LIBFT)/includes $(SRCS) -L$(LIBFT) -lft -lmlx -lXext -lX11 -lm

clean:
	#make clean -C $(LIBFT)
	@printf "Removing objects\n"
	@rm -f $(OBJS)

fclean : clean
	#make fclean -C $(LIBFT)
	@printf "Removing $(NAME)\n"
	@rm -f $(NAME)

re: fclean all

.PHONY: all libft clean fclean re
