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
#include "main_scene.h"

void	*main_scene_init(t_context *context, SDL_UNUSED void *level)
{
	t_main_scene	*scene;
	size_t			y_offset;

	scene = new_scene(sizeof(*scene), context, NULL, main_scene_close, main_scene_update);

	scene->min = 0;
	scene->max = WIN_WIDTH;
	y_offset = 80;

	int	side_padding = 20;
	SDLX_Button_Init(&(scene->increment), fetch_button_sprite, 0, (SDL_Rect){side_padding, 230 + y_offset, 32, 32}, NULL);
	SDLX_Button_Init(&(scene->decrement), fetch_button_sprite, 0, (SDL_Rect){WIN_WIDTH / 2 - side_padding - 32, 230 + y_offset, 32, 32}, NULL);
	SDLX_Style_Button(&(scene->increment), 0, 1);
	SDLX_Style_Button(&(scene->decrement), 0, 1);
	scene->increment.meta = &(scene->min);
	scene->decrement.meta = &(scene->max);
	scene->increment.trigger_fn = button_domain_change;
	scene->decrement.trigger_fn = button_domain_change;

	SDLX_Button_Init(&(scene->add), fetch_add_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 16, 150 + y_offset, 32, 32}, NULL);
	scene->add.sprite.angle = 45;
	scene->add.meta = &(scene->curves);
	scene->add.meta1 = &(scene->active_id);
	scene->add.trigger_fn = button_add_slider;

	create_text(&(scene->lower_bound),  0xFFFFFF00, (SDL_Rect){-50,  175 + 30 + y_offset, 0, 0}, "$$$$$$$$$$-1235789012345678901234567890", .15, context->font);
	create_text(&(scene->upper_bound),  0xFFFFFF00, (SDL_Rect){WIN_WIDTH / 2 - 120,  175 + 30 + y_offset, 0, 0}, "$$$$$$$$$$-1235789012345678901234567890", .15, context->font);

	scene->active = NULL;
	curves_init(&(scene->curves));
	curve_add(&(scene->curves), &(scene->active_id), 0xFFFFFF);

	SDLX_Button_Init(&(scene->sliders_start), fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	SDLX_Button_Init(&(scene->sliders_end),   fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	scene->sliders_start.sprite._dst.x = -9 * SLIDER_SCALE;
	scene->sliders_end.sprite._dst.x = WIN_WIDTH / 2 - 9 * SLIDER_SCALE;
	scene->color_start.s_color = 0xFF00FF;
	scene->color_end.s_color = 0x0000FF;

	SDLX_Button_Init(&(scene->slider_dec), fetch_slider_move_sprite, 2, (SDL_Rect){WIN_WIDTH / 4 - 8 - 32, 158 + y_offset, 16, 16}, NULL);
	SDLX_Button_Init(&(scene->slider_inc), fetch_slider_move_sprite, 3, (SDL_Rect){WIN_WIDTH / 4 - 8 + 32, 158 + y_offset, 16, 16}, NULL);
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

	SDLX_Button_Init(&(scene->paste), fetch_button_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 24, 215 + y_offset, 48, 48}, NULL);
	scene->paste.trigger_fn = button_paste;
	scene->paste.meta = &(scene->curves);
	scene->paste.meta = &(scene->active);

	SDLX_Button_Init(&(scene->save_file), fetch_save_sprite, 0, (SDL_Rect){150, 100, 32, 32}, NULL);
	scene->save_file.trigger_fn = button_generate;
	SDLX_Style_Button(&(scene->save_file), 0, 1);
	scene->save_file.meta = &(scene->curves);
	scene->save_file.meta1 = &(scene->active_id);

	SDLX_Button_Init(&(scene->view_file), fetch_save_sprite, 2, (SDL_Rect){150, 130, 32, 32}, NULL);
	scene->view_file.trigger_fn = button_view_output;
	SDLX_Style_Button(&(scene->view_file), 2, 3);
	scene->view_file.meta = &(scene->save_string);
	scene->view_file.meta1 = &(scene->popup.isDisabled);

	SDLX_Button_Init(&(scene->popup), fetch_button_sprite, 0, (SDL_Rect){(WIN_WIDTH / 2 - 175) / 2, 10, 175, 280}, NULL);
	scene->popup.isDisabled = SDL_TRUE;

	create_text(&(scene->auto_str), 0xBBBBBB00, (SDL_Rect){(WIN_WIDTH / 2 - 175) / 2 + 25, 10 + 40, 0, 0}, "one", .1, context->font);

	scene->save_string = SDL_calloc(5, sizeof(char));

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
	char		buff[30];

	scene = vp_scene;

	if (scene->popup.isDisabled == SDL_FALSE)
	{
		scene->auto_str.set = scene->save_string;
		update_text_wrapped(&(scene->auto_str), 0, 10000);
		SDLX_RenderQueue_Add(NULL, &(scene->auto_str.sprite));
	}
	update_buttons(scene);

	int i = 0;

	SDL_qsort(scene->curves.curves, scene->curves.curve_count, sizeof(*(scene->curves.curves)), clerp_sort);
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
		i += 5;
	}

	SDL_Renderer *renderer;
	SDL_Rect	color_box;

	renderer = SDLX_GetDisplay()->renderer;
	color_box.h = 17 * 2;
	color_box.w = 17 * 2;
	color_box.y = 60 + 2;

	i = 0;
	while (i < scene->curves.curve_count)
	{
		draw_slider_color(scene->curves.curves[i].s_color, &(color_box), scene->curves.curves[i].slider_b.sprite._dst.x, renderer);
		scene->curves.curves[i].slider_b.trigger_box = scene->curves.curves[i].slider_b.sprite._dst;
		SDLX_Button_Update(&(scene->curves.curves[i].slider_b));
		i++;
	}
	draw_slider_color(scene->color_start.s_color, &(color_box), scene->sliders_start.sprite._dst.x, renderer);
	draw_slider_color(scene->color_end.s_color, &(color_box), scene->sliders_end.sprite._dst.x, renderer);
	SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, 0, 0, 0, 0);


	SDL_snprintf(buff, sizeof(buff), "%10d %10d", scene->max, scene->min);
	buff[10] = '\0';
	scene->upper_bound.set = buff;
	scene->lower_bound.set = &(buff[11]);
	update_text(&(scene->upper_bound), sizeof(buff));
	update_text(&(scene->lower_bound), sizeof(buff));
	SDLX_RenderQueue_Add(NULL, &(scene->upper_bound.sprite));
	SDLX_RenderQueue_Add(NULL, &(scene->lower_bound.sprite));


	if (scene->save_file.isTriggered == SDL_TRUE)
	{
		generate_c_file(scene->curves.curves, scene->curves.curve_count, &(scene->color_start), &(scene->color_end), scene->min, scene->max, &(scene->save_string));
		scene->save_file.isTriggered = SDL_FALSE;
	}

	return (NULL);
}
