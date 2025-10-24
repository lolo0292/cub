#include "cub3d.h"

static int	blocked(t_game *g, double nx, double ny)
{
	int mx = (int)nx;
	int my = (int)ny;
	if (mx < 0 || my < 0 || mx >= g->map_w || my >= g->map_h)
		return (1);
	return (g->map[my][mx] == 1);
}

void	player_update(t_game *g, double dt)
{
	double sp = 3.0 * dt;
	double rx = 2.5 * dt;
	double nx, ny;

	if (g->key[XK_a]) { nx = g->px - g->dir_y * sp; ny = g->py + g->dir_x * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
	if (g->key[XK_d]) { nx = g->px + g->dir_y * sp; ny = g->py - g->dir_x * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
	if (g->key[XK_w]) { nx = g->px + g->dir_x * sp; ny = g->py + g->dir_y * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
	if (g->key[XK_s]) { nx = g->px - g->dir_x * sp; ny = g->py - g->dir_y * sp; if (!blocked(g, nx, g->py)) g->px = nx; if (!blocked(g, g->px, ny)) g->py = ny; }
	if (g->key[XK_Left])
	{
		double ox = g->dir_x, px = g->pl_x;
		g->dir_x = g->dir_x * cos(rx) - g->dir_y * sin(rx);
		g->dir_y = ox * sin(rx) + g->dir_y * cos(rx);
		g->pl_x = g->pl_x * cos(rx) - g->pl_y * sin(rx);
		g->pl_y = px * sin(rx) + g->pl_y * cos(rx);
	}
	if (g->key[XK_Right])
	{
		double ox = g->dir_x, px = g->pl_x;
		g->dir_x = g->dir_x * cos(-rx) - g->dir_y * sin(-rx);
		g->dir_y = ox * sin(-rx) + g->dir_y * cos(-rx);
		g->pl_x = g->pl_x * cos(-rx) - g->pl_y * sin(-rx);
		g->pl_y = px * sin(-rx) + g->pl_y * cos(-rx);
	}
}

int	key_press(int key, t_game *g) { if (key >= 0 && key < 512) g->key[key] = 1; if (key == XK_Escape) close_win(g); return (0); }
int	key_release(int key, t_game *g) { if (key >= 0 && key < 512) g->key[key] = 0; return (0); }
int	close_win(t_game *g) { destroy_textures(g); frame_destroy(g); mlx_destroy_window(g->mlx, g->win); exit(0); return (0); }
