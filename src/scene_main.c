/***************************************************************************
 * FILENAME:    scene_main.c
 * DESCRIPTION: The main scene for the gradient program.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 13Sep2021
***************************************************************************/

#include "main.h"

typedef struct	s_main_scene
{
	SDLX_button	increment;
	SDLX_button	decrement;

	SDLX_button	slider_inc;
	SDLX_button	slider_dec;
	SDLX_button	paste;

	SDLX_button	add;

	clerps		color_start;
	clerps		color_end;
	SDLX_button	sliders_start;
	SDLX_button	sliders_end;

	integer		min;
	integer		max;

	t_clerp_m	curves;
	int			active_id;
	clerps		*active;

	t_text		lower_bound;
	t_text		upper_bound;

	int			bar_c[WIN_WIDTH * 3];

	SDLX_button	save_file;

}				t_main_scene;

void	*main_scene_init(t_context *context, SDL_UNUSED void *level)
{
	t_main_scene	*scene;

	scene = new_scene(sizeof(*scene), context, NULL, main_scene_close, main_scene_update);

	scene->min = 0;
	scene->max = WIN_WIDTH;

	int	side_padding = 20;
	SDLX_Button_Init(&(scene->increment), fetch_button_sprite, 0, (SDL_Rect){side_padding, 230, 32, 32}, NULL);
	SDLX_Button_Init(&(scene->decrement), fetch_button_sprite, 0, (SDL_Rect){WIN_WIDTH / 2 - side_padding - 32, 230, 32, 32}, NULL);
	SDLX_Style_Button(&(scene->increment), 0, 1);
	SDLX_Style_Button(&(scene->decrement), 0, 1);
	scene->increment.meta = &(scene->min);
	scene->decrement.meta = &(scene->max);
	scene->increment.trigger_fn = button_domain_change;
	scene->decrement.trigger_fn = button_domain_change;

	SDLX_Button_Init(&(scene->add), fetch_add_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 16, 150, 32, 32}, NULL);
	scene->add.sprite.angle = 45;
	scene->add.meta = &(scene->curves);
	scene->add.meta1 = &(scene->active_id);
	scene->add.trigger_fn = button_add_slider;

	create_text(&(scene->lower_bound),  0xFFFFFF00, (SDL_Rect){-50,  175 + 30, 0, 0}, "$$$$$$$$$$-1235789012345678901234567890", .15, context->font);
	create_text(&(scene->upper_bound),  0xFFFFFF00, (SDL_Rect){WIN_WIDTH / 2 - 120,  175 + 30, 0, 0}, "$$$$$$$$$$-1235789012345678901234567890", .15, context->font);

	scene->active = NULL;
	curves_init(&(scene->curves));
	curve_add(&(scene->curves), &(scene->active_id), 0xFFFFFF);

	SDLX_Button_Init(&(scene->sliders_start), fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	SDLX_Button_Init(&(scene->sliders_end),   fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	scene->sliders_start.sprite._dst.x = -9 * SLIDER_SCALE;
	scene->sliders_end.sprite._dst.x = WIN_WIDTH / 2 - 9 * SLIDER_SCALE;
	scene->color_start.s_color = 0xFF00FF;
	scene->color_end.s_color = 0x0000FF;

	SDLX_Button_Init(&(scene->slider_dec), fetch_slider_move_sprite, 2, (SDL_Rect){WIN_WIDTH / 4 - 8 - 32, 158, 16, 16}, NULL);
	SDLX_Button_Init(&(scene->slider_inc), fetch_slider_move_sprite, 3, (SDL_Rect){WIN_WIDTH / 4 - 8 + 32, 158, 16, 16}, NULL);
	SDLX_Style_Button(&(scene->slider_dec), 2, 0);
	SDLX_Style_Button(&(scene->slider_inc), 3, 1);
	scene->slider_dec.left = &(scene->min);
	scene->slider_dec.right = &(scene->max);
	scene->slider_inc.left = &(scene->min);
	scene->slider_inc.right = &(scene->max);
	scene->slider_dec.meta = &(scene->active);
	scene->slider_inc.meta = &(scene->active);
	scene->slider_dec.meta_length = -5;
	scene->slider_inc.meta_length = 5;
	scene->slider_dec.trigger_fn = button_slider_change;
	scene->slider_inc.trigger_fn = button_slider_change;

	SDLX_Button_Init(&(scene->paste), fetch_button_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 24, 200, 48, 48}, NULL);
	scene->paste.trigger_fn = button_paste;
	scene->paste.meta = &(scene->curves);
	scene->paste.meta = &(scene->active);

	SDLX_Button_Init(&(scene->save_file), fetch_save_sprite, 0, (SDL_Rect){150, 100, 32, 32}, NULL);
	scene->save_file.trigger_fn = button_generate;
	SDLX_Style_Button(&(scene->save_file), 0, 1);
	scene->save_file.meta = &(scene->curves);
	scene->save_file.meta1 = &(scene->active_id);

	return (NULL);
}

void	*main_scene_close(SDL_UNUSED t_context *context, void *vp_scene)
{
	t_main_scene *scene;

	scene = vp_scene;

	SDLX_RenderQueue_Flush(NULL, SDLX_GetDisplay()->renderer, SDL_FALSE);
	SDL_free(context->background.sprite_data);
	SDL_free(scene);

	return (NULL);
}

void	*main_scene_update(SDL_UNUSED t_context *context, void *vp_scene)
{
	t_main_scene *scene;
	char		buff[20];

	scene = vp_scene;

	SDLX_Button_Update(&(scene->increment));
	SDLX_Button_Update(&(scene->decrement));
	SDLX_Button_Update(&(scene->slider_inc));
	SDLX_Button_Update(&(scene->slider_dec));
	SDLX_Button_Update(&(scene->sliders_start));
	SDLX_Button_Update(&(scene->sliders_end));
	SDLX_Button_Update(&(scene->paste));
	SDLX_Button_Update(&(scene->save_file));

	SDL_Rect bar;
	bar.x = scene->min;
	bar.w = scene->max - bar.x;
	bar.h = 50;
	bar.y = 100;

	SDLX_Button_Update(&(scene->add));
	SDL_qsort(scene->curves.curves, scene->curves.curve_count, sizeof(*(scene->curves.curves)), clerp_sort);

	int i = 0;
	while (i < scene->curves.curve_count)
	{
		fetch_slider_sprite(&(scene->curves.curves[i].slider_b.sprite.sprite_data), 0);
		if (scene->curves.curves[i].x == scene->active_id)
		{
			scene->active = &(scene->curves.curves[i]);
			fetch_slider_sprite(&(scene->curves.curves[i].slider_b.sprite.sprite_data), 1);
		}
		i++;
	}

	create_gradient(scene->bar_c, scene->curves.curve_count, scene->curves.curves, scene->color_start, scene->color_end);

	i = 0;
	int	r, g, b;

	int	ra[WIN_WIDTH];
	int	ga[WIN_WIDTH];
	int	ba[WIN_WIDTH];

	i = 0;
	while (i < WIN_WIDTH)
	{
		ctoRGB(scene->bar_c[i], &ra[i], &ga[i], &ba[i]);
		i++;
	}

	i = 0;
	while (i < WIN_WIDTH)
	{
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i + 0, 110, i + 0, 110 + 30);
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i + 1, 110, i + 1, 110 + 30);
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i + 2, 110, i + 2, 110 + 30);
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i + 3, 110, i + 3, 110 + 30);
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i + 4, 110, i + 4, 110 + 30);

		// SDL_RenderDrawLine(SDLX_GetDisplay()->renderer, i, 110, i, 110 + 30);
		i += 5;
	}

	SDL_Rect	color_box;
	color_box.h = 17 * 2;
	color_box.w = 17 * 2;
	color_box.y = 3;


	color_box.y = 60;
	color_box.x = scene->sliders_start.sprite._dst.x * 2 + 1;
	ctoRGB(scene->color_start.s_color, &r, &g, &b);
	SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, r, g, b, 0);
	SDL_RenderFillRect(SDLX_GetDisplay()->renderer, &(color_box));

	color_box.x = scene->sliders_end.sprite._dst.x * 2 + 1;
	ctoRGB(scene->color_end.s_color, &r, &g, &b);
	SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, r, g, b, 0);
	SDL_RenderFillRect(SDLX_GetDisplay()->renderer, &(color_box));

	i = 0;
	while (i < scene->curves.curve_count)
	{
		color_box.x = scene->curves.curves[i].slider_b.sprite._dst.x * 2 + 1;
		scene->curves.curves[i].slider_b.trigger_box = scene->curves.curves[i].slider_b.sprite._dst;
		ctoRGB(scene->curves.curves[i].s_color, &r, &g, &b);
		SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, r, g, b, 0);
		SDL_RenderFillRect(SDLX_GetDisplay()->renderer, &(color_box));
		SDLX_Button_Update(&(scene->curves.curves[i].slider_b));
		i++;
	}

	SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, 0, 0, 0, 0);

	SDL_snprintf(buff, sizeof(buff), "%10d", scene->max);
	scene->upper_bound.set = buff;
	update_text(&(scene->upper_bound), sizeof(buff));
	SDLX_RenderQueue_Add(NULL, &(scene->upper_bound.sprite));
	SDL_snprintf(buff, sizeof(buff), "%10d", scene->min);
	scene->lower_bound.set = buff;
	update_text(&(scene->lower_bound), sizeof(buff));
	SDLX_RenderQueue_Add(NULL, &(scene->lower_bound.sprite));

	if (scene->save_file.isTriggered == SDL_TRUE)
	{
		generate_c_file(scene->curves.curves, scene->curves.curve_count, &(scene->color_start), &(scene->color_end), scene->min, scene->max);
		scene->save_file.isTriggered = SDL_FALSE;
	}

	return (NULL);
}
