/***************************************************************************
 * FILENAME:    curves.c
 * DESCRIPTION:  Functions that are clerp managers.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 14Sep2021
***************************************************************************/

#include "main.h"

#define DEFAULT_CAP (100)

void	curves_init(t_clerp_m *manager)
{
	manager->curve_count = 0;
	manager->capacity = DEFAULT_CAP;
	manager->curves = SDL_calloc(manager->capacity, sizeof(*(manager->curves)));
}

void	curve_add(t_clerp_m *manager, int *active_id, int color)
{
	clerps	*elem;
	double	scale = SLIDER_SCALE;

	SDL_assert(manager->curve_count < manager->capacity);

	elem = &(manager->curves[manager->curve_count]);

	elem->s_color = color;
	SDLX_Button_Init(&(elem->slider_b), fetch_slider_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 9 * scale, 30, 18 * scale, 37 * scale}, NULL);
	elem->slider_b.sprite.dst = SDLX_NULL_SELF;
	elem->slider_b.trigger_fn = button_slider;
	elem->slider_b.meta = active_id;

	elem->x = manager->id_at;
	*active_id = manager->id_at;

	manager->curve_count++;
	manager->id_at++;
}

void	curve_remove(t_clerp_m *manager, int *active_id)
{
	int	ix;

	if (manager->curve_count <= 0)
	{
		*active_id = -1;
		return ;
	}

	ix = 0;
	while (ix < manager->capacity)
	{
		if (manager->curves[ix].x == *active_id)
			break ;
		ix++;
	}

	SDL_memmove(&(manager->curves[ix]), &(manager->curves[manager->curve_count - 1]), sizeof(*(manager->curves)));
	*active_id = manager->curves[0].x;

	if (manager->curve_count <= 1)
		*active_id = -1;
	manager->curve_count--;

}

void	grad_lerp(int *colors, int start, int end, int tmax, int offset)
{
	int	i;

	i = 0;
	while (i < tmax)
	{
		colors[i + offset] = clerp(start, end, i, tmax);
		i++;
	}
}

void	create_gradient_ends(int *dest, clerps color_start, clerps color_end)
{
	int	start_color;
	int	end_color;

	start_color = color_start.s_color;
	end_color = color_end.s_color;
	grad_lerp(dest, start_color, end_color, WIN_WIDTH, 0);
}

void	create_gradient(int *dest, int curve_amount, clerps *lerp_info, clerps color_start, clerps color_end)
{
	int	i;
	int	start_color;
	int	end_color;
	int	offset;
	int	distance;
	int	bias;

	if (curve_amount <= 0)
	{
		create_gradient_ends(dest, color_start, color_end);
		return ;
	}

	i = 0;
	offset = 0;
	bias = 9 / 2.0 * SLIDER_SCALE + 2;

	start_color = color_start.s_color;
	end_color = lerp_info[0].s_color;
	distance = (lerp_info[0].slider_b.sprite._dst.x - color_start.slider_b.sprite._dst.x) * 2 + bias + bias + 7;
	grad_lerp(dest, start_color, end_color, distance, offset);

	offset += distance;
	while (i < curve_amount - 1)
	{
		start_color = end_color;
		end_color = lerp_info[i + 1].s_color;
		distance = (lerp_info[i + 1].slider_b.sprite._dst.x - lerp_info[i].slider_b.sprite._dst.x) * 2;
		grad_lerp(dest, start_color, end_color, distance, offset);

		offset += distance;
		i++;
	}

	start_color = end_color;
	end_color = color_end.s_color;
	distance = (WIN_WIDTH / 2 - lerp_info[i].slider_b.sprite._dst.x) * 2;
	grad_lerp(dest, start_color, end_color, distance, offset);
}
