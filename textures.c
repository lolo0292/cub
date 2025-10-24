#include "cub3d.h"

static int	load_one(t_game *g, t_tex *t, const char *path)
{
	t->i.img = mlx_xpm_file_to_image(g->mlx, (char *)path, &t->i.w, &t->i.h);
	if (!t->i.img)
		return (0);
	t->i.addr = mlx_get_data_addr(t->i.img, &t->i.bpp, &t->i.line_len, &t->i.endian);
	return (t->i.addr != NULL);
}

static void	gen_checker(t_game *g, t_tex *t, int w, int h)
{
	int x, y;

	(void)g;
	t->i.img = mlx_new_image(g->mlx, w, h);
	t->i.addr = mlx_get_data_addr(t->i.img, &t->i.bpp, &t->i.line_len, &t->i.endian);
	t->i.w = w;
	t->i.h = h;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			if (((x / 8) + (y / 8)) % 2)
				*(uint32_t *)(t->i.addr + y * t->i.line_len + x * (t->i.bpp / 8)) = 0xFF5555;
			else
				*(uint32_t *)(t->i.addr + y * t->i.line_len + x * (t->i.bpp / 8)) = 0xAAAAAA;
			x++;
		}
		y++;
	}
}

int	load_textures(t_game *g, const char *no, const char *so, const char *we, const char *ea)
{
	if (!load_one(g, &g->tex[TEX_NO], no)) gen_checker(g, &g->tex[TEX_NO], 64, 64);
	if (!load_one(g, &g->tex[TEX_SO], so)) gen_checker(g, &g->tex[TEX_SO], 64, 64);
	if (!load_one(g, &g->tex[TEX_WE], we)) gen_checker(g, &g->tex[TEX_WE], 64, 64);
	if (!load_one(g, &g->tex[TEX_EA], ea)) gen_checker(g, &g->tex[TEX_EA], 64, 64);
	return (1);
}

void	destroy_textures(t_game *g)
{
	int i;

	i = 0;
	while (i < TEX_NB)
	{
		if (g->tex[i].i.img)
			mlx_destroy_image(g->mlx, g->tex[i].i.img);
		i++;
	}
}

uint32_t	get_texel(const t_tex *t, int u, int v)
{
	char *p;

	if (u < 0) u = 0;
	if (v < 0) v = 0;
	if (u >= t->i.w) u = t->i.w - 1;
	if (v >= t->i.h) v = t->i.h - 1;
	p = t->i.addr + v * t->i.line_len + u * (t->i.bpp / 8);
	return (*(uint32_t *)p);
}

int	choose_tex_id(const t_ray *r)
{
	if (r->side == 0)
		return (r->ray_x < 0 ? TEX_WE : TEX_EA);
	return (r->ray_y < 0 ? TEX_NO : TEX_SO);
}

static void gen_floor_checker(t_game *g, t_tex *t, int w, int h)
{
	int x, y;

	(void)g;
	t->i.img = mlx_new_image(g->mlx, w, h);
	t->i.addr = mlx_get_data_addr(t->i.img, &t->i.bpp, &t->i.line_len, &t->i.endian);
	t->i.w = w;
	t->i.h = h;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			if (((x / 8) + (y / 8)) % 2)
				*(uint32_t *)(t->i.addr + y * t->i.line_len + x * (t->i.bpp / 8)) = 0x3A6B2E;
			else
				*(uint32_t *)(t->i.addr + y * t->i.line_len + x * (t->i.bpp / 8)) = 0x2A4D1A;
			x++;
		}
		y++;
	}
}

int	load_floor_texture(t_game *g, const char *path)
{
	if (!load_one(g, &g->floor_tex, path))
		gen_floor_checker(g, &g->floor_tex, 64, 64);
	return (1);
}
