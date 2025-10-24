#include "cub3d.h"


//ouvre la co cree la fenetre
int	init_mlx(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (0);
	g->win = mlx_new_window(g->mlx, W, H, "ray-only");
	return (g->win != NULL);
}

//cree l image
int	frame_new(t_game *g, int w, int h)
{
	t_img *f = &g->frame;

	f->img = mlx_new_image(g->mlx, w, h);
	if (!f->img)
		return (0);
	f->addr = mlx_get_data_addr(f->img, &f->bpp, &f->line_len, &f->endian);
	f->w = w;
	f->h = h;
	return (f->addr != NULL);
}

//cree l image
void	frame_destroy(t_game *g)
{
	if (g->frame.img)
		mlx_destroy_image(g->mlx, g->frame.img);
}

//met un pixel dans t img
void	put_px(t_img *i, int x, int y, uint32_t c)
{
	char *p;

	if (x < 0 || y < 0 || x >= i->w || y >= i->h)
		return ;
	p = i->addr + y * i->line_len + x * (i->bpp / 8);
	*(uint32_t *)p = c;
}
