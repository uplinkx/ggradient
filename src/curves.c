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