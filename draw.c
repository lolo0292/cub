// #include "cub3d.h"

// static void	draw_span(t_game *g, int x, int y0, int y1, uint32_t c)
// {
// 	if (y0 < 0) y0 = 0;
// 	if (y1 >= H) y1 = H - 1;
// 	while (y0 <= y1)
// 	{
// 		put_px(&g->frame, x, y0, c);
// 		y0++;
// 	}
// }

// static void	draw_walls(t_game *g, int x, t_ray *r, int h, int tex_id)
// {
// 	int		y;
// 	double	step;
// 	double	tex_pos;
// 	int		ty;
// 	int		tx;

// 	if (h < 1) h = 1;
// 	y = -h / 2 + H / 2;
//     tex_pos = 0.0;
//     if (y < 0)
// 	tex_pos = (-y) * (double)g->tex[tex_id].i.h / h;	
//     step = (double)g->tex[tex_id].i.h / (double)h;
// 	if (r->side == 0)
// 		tex_pos += 0;
// 	if (r->side == 0)
// 		tx = (int)((g->py + r->perp * r->ray_y) * g->tex[tex_id].i.w) % g->tex[tex_id].i.w;
// 	else
// 		tx = (int)((g->px + r->perp * r->ray_x) * g->tex[tex_id].i.w) % g->tex[tex_id].i.w;
// 	if ((r->side == 0 && r->ray_x > 0) || (r->side == 1 && r->ray_y < 0))
// 		tx = g->tex[tex_id].i.w - 1 - tx;
// 	while (y < H / 2 + h / 2)
// 	{
// 		ty = (int)tex_pos;
// 		put_px(&g->frame, x, y, get_texel(&g->tex[tex_id], tx, ty));
// 		tex_pos += step;
// 		y++;
// 	}
// }

// void	draw_column(t_game *g, int x, t_ray *r)
// {
// 	int		line_h;
// 	int		tex_id;
// 	int		ceil_end;
// 	int		floor_begin;

// 	line_h = (int)((double)H / fmax(r->perp, 1e-6));
// 	tex_id = choose_tex_id(r);
// 	ceil_end = -line_h / 2 + H / 2 - 1;
// 	floor_begin = line_h / 2 + H / 2;
// 	draw_span(g, x, 0, ceil_end, g->ceil_col);
// 	draw_walls(g, x, r, line_h, tex_id);
// 	draw_span(g, x, floor_begin, H - 1, g->floor_col);
// }

#include "cub3d.h"

static void	draw_span(t_game *g, int x, int y0, int y1, uint32_t c)
{
	if (y0 < 0)
		y0 = 0;
	if (y1 >= H)
		y1 = H - 1;
	while (y0 <= y1)
	{
		put_px(&g->frame, x, y0, c);
		y0++;
	}
}

static void	draw_walls(t_game *g, int x, t_ray *r, int h, int tex_id)
{
	int		y;
	double	step;
	double	tex_pos;
	int		ty;
	int		tx;

	if (h < 1)
		h = 1;
	y = -h / 2 + H / 2;
	tex_pos = 0.0;
	if (y < 0)
	{
		tex_pos = (-y) * (double)g->tex[tex_id].i.h / h;
		y = 0;
	}
	step = (double)g->tex[tex_id].i.h / (double)h;
	if (r->side == 0)
		tx = (int)((g->py + r->perp * r->ray_y) * g->tex[tex_id].i.w) % g->tex[tex_id].i.w;
	else
		tx = (int)((g->px + r->perp * r->ray_x) * g->tex[tex_id].i.w) % g->tex[tex_id].i.w;
	if ((r->side == 0 && r->ray_x > 0) || (r->side == 1 && r->ray_y < 0))
		tx = g->tex[tex_id].i.w - 1 - tx;
	while (y < H / 2 + h / 2 && y < H)
	{
		ty = (int)tex_pos;
		put_px(&g->frame, x, y, get_texel(&g->tex[tex_id], tx, ty));
		tex_pos += step;
		y++;
	}
}

static void	draw_floor_column(t_game *g, int x, t_ray *r, int floor_begin)
{
	double	wallX;
	double	floorXWall;
	double	floorYWall;
	int		y;

	if (floor_begin < 0)
		floor_begin = 0;
	if (r->side == 0)
		wallX = g->py + r->perp * r->ray_y;
	else
		wallX = g->px + r->perp * r->ray_x;
	wallX -= floor(wallX);
	if (r->side == 0 && r->ray_x > 0)
	{
		floorXWall = r->map_x;
		floorYWall = r->map_y + wallX;
	}
	else if (r->side == 0 && r->ray_x < 0)
	{
		floorXWall = r->map_x + 1.0;
		floorYWall = r->map_y + wallX;
	}
	else if (r->side == 1 && r->ray_y > 0)
	{
		floorXWall = r->map_x + wallX;
		floorYWall = r->map_y;
	}
	else
	{
		floorXWall = r->map_x + wallX;
		floorYWall = r->map_y + 1.0;
	}
	y = floor_begin;
	while (y < H)
	{
		double curDist = (double)H / (2.0 * y - H + 0.00001);
		double w = curDist / fmax(r->perp, 1e-6);
		double curX = w * floorXWall + (1.0 - w) * g->px;
		double curY = w * floorYWall + (1.0 - w) * g->py;
		int tx = (int)(curX * g->floor_tex.i.w) % g->floor_tex.i.w;
		int ty = (int)(curY * g->floor_tex.i.h) % g->floor_tex.i.h;
		if (tx < 0)
			tx += g->floor_tex.i.w;
		if (ty < 0)
			ty += g->floor_tex.i.h;
		put_px(&g->frame, x, y, get_texel(&g->floor_tex, tx, ty));
		y++;
	}
}

void	draw_column(t_game *g, int x, t_ray *r)
{
	int		line_h;
	int		tex_id;
	int		ceil_end;
	int		floor_begin;

	line_h = (int)((double)H / fmax(r->perp, 1e-6));
	tex_id = choose_tex_id(r);
	ceil_end = -line_h / 2 + H / 2 - 1;
	floor_begin = line_h / 2 + H / 2;
	draw_span(g, x, 0, ceil_end, g->ceil_col);
	draw_walls(g, x, r, line_h, tex_id);
	if (floor_begin < H)
		draw_floor_column(g, x, r, floor_begin);
}
