NAME      := ray
CC        := cc
CFLAGS    := -Wall -Wextra -Werror -O2 -I. -Iminilibx-linux
SRCS_DIR  := .
OBJ_DIR   := obj
MLX_DIR   := minilibx-linux

SRCS := main.c render.c raycast.c draw.c textures.c player.c mlx_wrap.c
OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

MLX_LIB_STD   := $(MLX_DIR)/libmlx.a
MLX_LIB_LNX   := $(MLX_DIR)/libmlx_Linux.a
MLX_LIB       := $(firstword $(wildcard $(MLX_LIB_LNX) $(MLX_LIB_STD)))

ifeq ($(notdir $(MLX_LIB)),libmlx_Linux.a)
  MLXFLAGS := $(MLX_LIB) -lXext -lX11 -lm
else
  MLXFLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm
endif

all: $(NAME)

$(MLX_LIB_STD) $(MLX_LIB_LNX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJS) $(MLX_LIB_STD) $(MLX_LIB_LNX)
	$(CC) $(CFLAGS) $(OBJS) $(MLXFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c cub3d.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
