#include "cub3d.h"

static int	blocked(t_game *g, double nx, double ny)
{
	int mx = (int)nx;
	int my = (int)ny;
	if (mx < 0 || my < 0 || mx >= g->map_w || my >= g->map_h)
		return (1);
	return (g->map[my][mx] == 1);
}

// void	player_update(t_game *g, double dt)
// {
// 	double sp = 3.0 * dt;
// 	double rx = 2.5 * dt;
// 	double nx, ny;

// 	if (g->key[XK_a]) { nx = g->px - g->dir_y * sp; ny = g->py + g->dir_x * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
// 	if (g->key[XK_d]) { nx = g->px + g->dir_y * sp; ny = g->py - g->dir_x * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
// 	if (g->key[XK_w]) { nx = g->px + g->dir_x * sp; ny = g->py + g->dir_y * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
// 	if (g->key[XK_s]) { nx = g->px - g->dir_x * sp; ny = g->py - g->dir_y * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
// 	if (g->key[XK_Left])
// 	{
// 		double ox = g->dir_x, px = g->pl_x;
// 		g->dir_x = g->dir_x * cos(rx) - g->dir_y * sin(rx);
// 		g->dir_y = ox * sin(rx) + g->dir_y * cos(rx);
// 		g->pl_x = g->pl_x * cos(rx) - g->pl_y * sin(rx);
// 		g->pl_y = px * sin(rx) + g->pl_y * cos(rx);
// 	}
// 	if (g->key[XK_Right])
// 	{
// 		double ox = g->dir_x, px = g->pl_x;
// 		g->dir_x = g->dir_x * cos(-rx) - g->dir_y * sin(-rx);
// 		g->dir_y = ox * sin(-rx) + g->dir_y * cos(-rx);
// 		g->pl_x = g->pl_x * cos(-rx) - g->pl_y * sin(-rx);
// 		g->pl_y = px * sin(-rx) + g->pl_y * cos(-rx);
// 	}
// }

void	player_update(t_game *g, double dt)
{
	double	sp = 3.0 * dt;
	double	nx, ny;

	/* avant / arrière (W ou Z) et (S) */
	if (g->key[XK_w] || g->key[XK_z])
	{
		nx = g->px + g->dir_x * sp;
		ny = g->py + g->dir_y * sp;
		if (!blocked(g, nx, g->py)) g->px = nx;
		if (!blocked(g, g->px, ny)) g->py = ny;
	}
	if (g->key[XK_s])
	{
		nx = g->px - g->dir_x * sp;
		ny = g->py - g->dir_y * sp;
		if (!blocked(g, nx, g->py)) g->px = nx;
		if (!blocked(g, g->px, ny)) g->py = ny;
	}

	/* strafe gauche (A ou Q) et droite (D) — corrigé */
	if (g->key[XK_a] || g->key[XK_q])
	{
		nx = g->px + g->dir_y * sp;  /* gauche = +dir_y, -dir_x */
		ny = g->py - g->dir_x * sp;
		if (!blocked(g, nx, g->py)) g->px = nx;
		if (!blocked(g, g->px, ny)) g->py = ny;
	}
	if (g->key[XK_d])
	{
		nx = g->px - g->dir_y * sp;  /* droite = -dir_y, +dir_x */
		ny = g->py + g->dir_x * sp;
		if (!blocked(g, nx, g->py)) g->px = nx;
		if (!blocked(g, g->px, ny)) g->py = ny;
	}

	if (g->key[XK_Left])
		rotate_player(g, +2.5 * dt);
	if (g->key[XK_Right])
		rotate_player(g, -2.5 * dt);
}


int	key_press(int key, t_game *g) { if (key >= 0 && key < 512) g->key[key] = 1; if (key == XK_Escape) close_win(g); return (0); }
int	key_release(int key, t_game *g) { if (key >= 0 && key < 512) g->key[key] = 0; return (0); }
int	close_win(t_game *g) { destroy_textures(g); frame_destroy(g); mlx_destroy_window(g->mlx, g->win); exit(0); return (0); }


// extern int mlx_mouse_hide(void *mlx);     /* fournis par minilibx-linux */
extern int mlx_mouse_show(void *mlx, void *win);
extern int mlx_mouse_move(void *mlx, void *win, int x, int y);

void	rotate_player(t_game *g, double a)
{
	double ox;
	double px;

	ox = g->dir_x;
	px = g->pl_x;
	g->dir_x = g->dir_x * cos(a) - g->dir_y * sin(a);
	g->dir_y = ox * sin(a) + g->dir_y * cos(a);
	g->pl_x = g->pl_x * cos(a) - g->pl_y * sin(a);
	g->pl_y = px * sin(a) + g->pl_y * cos(a);
}

void	center_mouse(t_game *g)
{
	mlx_mouse_move(g->mlx, g->win, W / 2, H / 2);
}

int	mouse_press(int button, int x, int y, t_game *g)
{
	(void)x;
	(void)y;
	if (button == 1 && !g->mouse_captured)
	{
		g->mouse_captured = 1;
		mlx_mouse_hide(g->mlx, g->win);   // <-- 2 args
		center_mouse(g);
	}
	return (0);
}

int	mouse_release(int button, int x, int y, t_game *g)
{
	(void)x;
	(void)y;
	if (button == 3 && g->mouse_captured)
	{
		g->mouse_captured = 0;
		mlx_mouse_show(g->mlx, g->win);   // <-- 2 args
	}
	return (0);
}

int	mouse_move(int x, int y, t_game *g)
{
	int		dx;
	double	a;

	(void)y;
	if (!g->mouse_captured)
		return (0);
	dx = x - (W / 2);
	if (dx != 0)
	{
		a = (double)dx * g->mouse_sens;
		rotate_player(g, a);
		center_mouse(g);
	}
	return (0);
}
