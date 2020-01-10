# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/05 13:58:13 by wkorande          #+#    #+#              #
#    Updated: 2020/01/10 10:22:16 by wkorande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

CFLAGS = -Wall -Wextra -Werror

SRC = rtv1.c\
	env.c\
	mlx_image.c\
	vec3.c\
	ray.c

SRCDIR = src

SRCS = $(addprefix $(SRCDIR)/, $(SRC))

OBJS = $(SRC:.c=.o)

INCL = include

LIBFT = ../libft

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	make -C $(LIBFT)
	gcc $(FLAGS) -o $(NAME) -I$(INCL) -I $(LIBFT)/includes $(SRCS) -L$(LIBFT) -lft -lmlx -lm -pthread -framework OpenGL -framework AppKit

linux:
	make -C $(LIBFT)
	gcc $(FLAGS) -o $(NAME) -I$(INCL) -I $(LIBFT)/includes $(SRCS) -L$(LIBFT) -lft -lmlx -lXext -lX11 -lm

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
