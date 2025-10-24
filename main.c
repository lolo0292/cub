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

// int	setup_demo_world(t_game *g)
// {
// 		const char *rows[] = {
//         "1111111111111111111111111",
//         "1000000000110000000000001",
//         "1011000001110000000000001",
//         "1001000000000000000000001",
// "11111 111111000001110000000000001",
// "100000000011000001110111111111111",
// "11111111111111011100000010001",
// "11110111111111011101010010001",
// "11000000110101011100000010001",
// "100000000000000011000N0010001",
// "10000000000000001101010010001",
// "1100000111010101111101111000111",
// "11110111 1110101 101111010001",
// "11111111 1111111 111111111111"
// 	};
// 	int w = 12, h = 7;

// 	g->map_w = w;
// 	g->map_h = h;
// 	g->map = alloc_map(w, h);
// 	fill_demo(g->map, w, h, rows);
// 	g->px = 2.5; g->py = 1.5;
// 	g->dir_x = 1.0; g->dir_y = 0.0;
// 	g->pl_x = 0.0; g->pl_y = 0.66;
// 	g->ceil_col = 0x87CEEB;
// 	g->floor_col = 0x444444;
// 	return (load_textures(g, "north.xpm", "south.xpm", "west.xpm", "east.xpm"));
// }

int	setup_demo_world(t_game *g)
{
	const char *rows[] = {
		"1111111111111111111111111",
		"1000000000110000000000001",
		"101100000111000N000000001",
		"1001000000000000000000001",
		"1111111111110000011100001",
		"1000000000110000011101111",
		"1111111111111101110000001",
		"1111011111111101110101001",
		"1100000011010101110000001",
		"1000000000000000110000001",
		"1000000000000000110101001",
		"1100000111010101111101111",
		"1111011111101011011110101",
		"1111111111111111111111111"
	};
	int w = 25;
	int h = 14;

	g->map_w = w;
	g->map_h = h;
	g->map = alloc_map(w, h);
	fill_demo(g->map, w, h, rows);
	g->px = 11.5;
	g->py = 9.5;
	g->dir_x = 0.0;
	g->dir_y = -1.0;
	g->pl_x = 0.66;
	g->pl_y = 0.0;
	g->ceil_col = 0x8FB3D9;
	g->floor_col = 0x444444;
	if (!load_textures(g, "castle.xpm", "castle.xpm", "castle.xpm", "castle.xpm"))
		return (0);
	printf("castle: NO=%p SO=%p WE=%p EA=%p\n",
    g->tex[0].i.addr, g->tex[1].i.addr, g->tex[2].i.addr, g->tex[3].i.addr);
	if (!load_floor_texture(g, "grass.xpm"))
		return (0);
	return (1);
}

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

int	loop_hook(t_game *g)
{
	static double t0 = 0.0;
	double t1, dt;

	t1 = get_time();
	if (t0 == 0.0) t0 = t1;
	dt = t1 - t0;
	t0 = t1;
	player_update(g, dt);
	render_frame(g);
	return (0);
}

// int	main(void)
// {
// 	t_game g;

// 	memset(&g, 0, sizeof(g));
// 	if (!init_mlx(&g) || !frame_new(&g, W, H))
// 		return (1);
// 	if (!setup_demo_world(&g))
// 		return (1);
// 	mlx_hook(g.win, 17, 0, close_win, &g);
// 	mlx_hook(g.win, 2, 1L<<0, key_press, &g);
// 	mlx_hook(g.win, 3, 1L<<1, key_release, &g);
// 	mlx_loop_hook(g.mlx, loop_hook, &g);
// 	mlx_loop(g.mlx);
// 	return (0);
// }

int	main(void)
{
	t_game	g;

	memset(&g, 0, sizeof(g));
	g.mouse_sens = 0.0025;
	g.mouse_captured = 0;
	if (!init_mlx(&g) || !frame_new(&g, W, H))
		return (1);
	if (!setup_demo_world(&g))
		return (1);
	mlx_hook(g.win, 17, 0, close_win, &g);
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_hook(g.win, 4, 1L << 2, mouse_press, &g);
	mlx_hook(g.win, 5, 1L << 3, mouse_release, &g);
	mlx_hook(g.win, 6, 1L << 6, mouse_move, &g);
	mlx_loop_hook(g.mlx, loop_hook, &g);
	mlx_loop(g.mlx);
	return (0);
}