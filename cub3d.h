#ifndef CUB3D_H
# define CUB3D_H
# include <stdlib.h>
# include <stdint.h>
# include <math.h>
# include <X11/keysym.h>
# include "mlx.h"

# define W 1024
# define H 640
# define TEX_NO 0
# define TEX_SO 1
# define TEX_WE 2
# define TEX_EA 3
# define TEX_NB 4

//mlx+image
typedef struct s_img {
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		w;
	int		h;
}	t_img;

//pr texture
typedef struct s_tex {
	t_img	i;
}	t_tex;

//var locale pour les rayons etc
typedef struct s_ray {
	double	cam_x;
	double	ray_x;
	double	ray_y;
	int		map_x;
	int		map_y;
	double	side_x;
	double	side_y;
	double	delta_x;
	double	delta_y;
	int		step_x;
	int		step_y;
	int		side;     /* 0=x,1=y */
	double	perp;     /* distance perpendiculaire */
}	t_ray;

//jeu global
typedef struct s_game {
	void	*mlx;
	void	*win;
	t_img	frame;
	t_tex	tex[TEX_NB];
	int		**map;
	int		map_w;
	int		map_h;
	double	px;
	double	py;
	double	dir_x;
	double	dir_y;
	double	pl_x;
	double	pl_y;
	uint32_t ceil_col;
	uint32_t floor_col;
	int		key[512];
}	t_game;

/* mlx_wrap.c */
int		init_mlx(t_game *g);
int		frame_new(t_game *g, int w, int h);
void	frame_destroy(t_game *g);
void	put_px(t_img *i, int x, int y, uint32_t c);

/* textures.c */
int		load_textures(t_game *g, const char *no, const char *so,
			const char *we, const char *ea);
void	destroy_textures(t_game *g);
uint32_t get_texel(const t_tex *t, int u, int v);
int		choose_tex_id(const t_ray *r);

/* render.c */
void	render_frame(t_game *g);

/* raycast.c */
void	ray_setup(t_game *g, t_ray *r, int x);
void	ray_dda(t_game *g, t_ray *r);

/* draw.c */
void	draw_column(t_game *g, int x, t_ray *r);

/* player.c */
int		key_press(int key, t_game *g);
int		key_release(int key, t_game *g);
int		close_win(t_game *g);
void	player_update(t_game *g, double dt);

/* main.c */
int		loop_hook(t_game *g);
int		setup_demo_world(t_game *g);
#endif
