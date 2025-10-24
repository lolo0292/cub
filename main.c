#include "cub3d.h"

static int	**alloc_map(int w, int h)
{
	int **m;
	int y;

	m = malloc(sizeof(int *) * h);
	y = 0;
	while (y < h)
	{
		m[y] = malloc(sizeof(int) * w);
		y++;
	}
	return (m);
}

static void	fill_demo(int **m, int w, int h, const char *rows[])
{
	int x, y;

	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			m[y][x] = (rows[y][x] == '1');
			x++;
		}
		y++;
	}
}

int	setup_demo_world(t_game *g)
{
	const char *rows[] = {
		"111111111111",
		"100000000001",
		"101111011101",
		"100100010001",
		"101100010101",
		"100000000001",
		"111111111111"
	};
	int w = 12, h = 7;

	g->map_w = w;
	g->map_h = h;
	g->map = alloc_map(w, h);
	fill_demo(g->map, w, h, rows);
	g->px = 2.5; g->py = 2.5;
	g->dir_x = 1.0; g->dir_y = 0.0;
	g->pl_x = 0.0; g->pl_y = 0.66;
	g->ceil_col = 0x87CEEB;
	g->floor_col = 0x444444;
	return (load_textures(g, "north.xpm", "south.xpm", "west.xpm", "east.xpm"));
}

int	loop_hook(t_game *g)
{
	static double t0 = 0.0;
	double t1, dt;

	t1 = (double)mlx_get_time();
	if (t0 == 0.0) t0 = t1;
	dt = t1 - t0;
	t0 = t1;
	player_update(g, dt);
	render_frame(g);
	return (0);
}

int	main(void)
{
	t_game g;

	if (!init_mlx(&g) || !frame_new(&g, W, H))
		return (1);
	if (!setup_demo_world(&g))
		return (1);
	mlx_hook(g.win, 17, 0, close_win, &g);
	mlx_hook(g.win, 2, 1L<<0, key_press, &g);
	mlx_hook(g.win, 3, 1L<<1, key_release, &g);
	mlx_loop_hook(g.mlx, loop_hook, &g);
	mlx_loop(g.mlx);
	return (0);
}
