#include "cub3d.h"

static void	set_steps(t_ray *r, double px, double py)
{
	if (r->ray_x < 0)
	{
		r->step_x = -1;
		r->side_x = (px - r->map_x) * r->delta_x;
	}
	else
	{
		r->step_x = 1;
		r->side_x = (r->map_x + 1.0 - px) * r->delta_x;
	}
	if (r->ray_y < 0)
	{
		r->step_y = -1;
		r->side_y = (py - r->map_y) * r->delta_y;
	}
	else
	{
		r->step_y = 1;
		r->side_y = (r->map_y + 1.0 - py) * r->delta_y;
	}
}

//calcule la cam pour x
void	ray_setup(t_game *g, t_ray *r, int x)
{
	r->cam_x = 2.0 * x / (double)W - 1.0;
	r->ray_x = g->dir_x + g->pl_x * r->cam_x;
	r->ray_y = g->dir_y + g->pl_y * r->cam_x;
	r->map_x = (int)g->px;
	r->map_y = (int)g->py;
	r->delta_x = (r->ray_x == 0) ? 1e30 : fabs(1.0 / r->ray_x);
	r->delta_y = (r->ray_y == 0) ? 1e30 : fabs(1.0 / r->ray_y);
	set_steps(r, g->px, g->py);
}

//avance case par case
void	ray_dda(t_game *g, t_ray *r)
{
	int hit;

	hit = 0;
	while (!hit)
	{
		if (r->side_x < r->side_y)
		{
			r->side_x += r->delta_x;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_y += r->delta_y;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (r->map_x < 0 || r->map_y < 0 || r->map_x >= g->map_w || r->map_y >= g->map_h)
			break ;
		if (g->map[r->map_y][r->map_x] == 1)
			hit = 1;
	}
	if (r->side == 0)
		r->perp = (r->side_x - r->delta_x);
	else
		r->perp = (r->side_y - r->delta_y);
}
