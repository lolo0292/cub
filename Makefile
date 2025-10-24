# Raycasting-only build (Linux + minilibx-linux)
NAME    := ray
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -O2
MLXDIR  := minilibx-linux
MLXINC  := -I$(MLXDIR)
MLXLIB  := -L$(MLXDIR) -lmlx -lXext -lX11 -lm
INC     := -I.
SRC     := main.c render.c raycast.c draw.c textures.c player.c mlx_wrap.c
OBJ     := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(MLXLIB) -o $@

%.o: %.c cub3d.h
	$(CC) $(CFLAGS) $(INC) $(MLXINC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
