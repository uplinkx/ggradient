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

	y_offset = -14;

	SDLX_Button_Init(&(scene->add), fetch_add_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 8 - 12, 192 + y_offset, 16, 16}, NULL);
	SDLX_Style_Button(&(scene->add), 0, 1);
	scene->add.meta = &(scene->curves);
	scene->add.meta1 = &(scene->active_id);
	scene->add.trigger_fn = button_add_slider;

	SDLX_Button_Init(&(scene->del), fetch_add_sprite, 2, (SDL_Rect){WIN_WIDTH / 4 - 8 + 12, 192 + y_offset, 16, 16}, NULL);
	SDLX_Style_Button(&(scene->del), 2, 3);
	scene->del.meta = &(scene->curves);
	scene->del.meta1 = &(scene->active_id);
	scene->del.trigger_fn = button_remove_slider;

	scene->active = NULL;
	curves_init(&(scene->curves));
	// curve_add(&(scene->curves), &(scene->active_id), 0xff3083);
	curve_add(&(scene->curves), &(scene->active_id), 0x000000);

	SDLX_Button_Init(&(scene->sliders_start), fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	SDLX_Button_Init(&(scene->sliders_end),   fetch_slider_sprite, 0, (SDL_Rect){WIN_HEIGHT / 2 - 16, 30, 18 * SLIDER_SCALE, 37 * SLIDER_SCALE}, NULL);
	scene->sliders_start.sprite._dst.x = -9 * SLIDER_SCALE;
	scene->sliders_end.sprite._dst.x = WIN_WIDTH / 2 - 9 * SLIDER_SCALE;
	scene->color_start.s_color = 0x3b3638;
	scene->color_end.s_color = 0xFFFFFF;

	scene->sliders_start.meta = &(scene->paste_meta.which);
	scene->sliders_start.meta1 = &(scene->active);
	scene->sliders_end.meta = &(scene->paste_meta.which);
	scene->sliders_end.meta1 = &(scene->active);

	scene->sliders_start.meta_length = 1;
	scene->sliders_end.meta_length = 2;
	scene->sliders_start.trigger_fn = button_ends;
	scene->sliders_end.trigger_fn = button_ends;
	scene->sliders_start.trigger_box = scene->sliders_start.sprite._dst;
	scene->sliders_end.trigger_box = scene->sliders_end.sprite._dst;

	SDLX_Button_Init(&(scene->slider_dec), fetch_slider_move_sprite, 2, (SDL_Rect){WIN_WIDTH / 4 - 8 - 32, 158 + y_offset, 16, 16}, NULL);
	SDLX_Button_Init(&(scene->slider_inc), fetch_slider_move_sprite, 3, (SDL_Rect){WIN_WIDTH / 4 - 8 + 32, 158 + y_offset, 16, 16}, NULL);
	SDLX_Style_Button(&(scene->slider_dec), 2, 0);
	SDLX_Style_Button(&(scene->slider_inc), 3, 1);
	scene->slider_dec.meta = &(scene->active);
	scene->slider_inc.meta = &(scene->active);
	scene->slider_dec.meta_length = -5;
	scene->slider_inc.meta_length = 5;
	scene->slider_dec.trigger_fn = button_slider_change;
	scene->slider_inc.trigger_fn = button_slider_change;

	SDLX_Button_Init(&(scene->paste), fetch_button_sprite, 0, (SDL_Rect){WIN_WIDTH / 4 - 24, 140 + y_offset, 48, 48}, NULL);
	SDLX_Style_Button(&(scene->paste), 0, 1);
	scene->paste.trigger_fn = button_paste;
	scene->paste.meta = &(scene->active);
	scene->paste.meta1 = &(scene->paste_meta);

	scene->paste_meta.which = 0;
	scene->paste_meta.ptr1 = &(scene->color_start.s_color);
	scene->paste_meta.ptr2 = &(scene->color_end.s_color);

	SDLX_Button_Init(&(scene->save_file), fetch_save_sprite, 0, (SDL_Rect){215, 164, 32, 32}, NULL);
	scene->save_file.trigger_fn = button_generate;
	SDLX_Style_Button(&(scene->save_file), 0, 1);
	scene->save_file.meta = &(scene->curves);
	scene->save_file.meta1 = &(scene->active_id);

	fetch_slider_sprite(&(scene->color_selector.r.sprite_data), 2);
	fetch_slider_sprite(&(scene->color_selector.g.sprite_data), 2);
	fetch_slider_sprite(&(scene->color_selector.b.sprite_data), 2);
	scene->color_selector.r.dst = SDLX_NULL_SELF;
	scene->color_selector.g.dst = SDLX_NULL_SELF;
	scene->color_selector.b.dst = SDLX_NULL_SELF;
	scene->color_selector.r._dst = (SDL_Rect){7, 	  100, 14, 8};
	scene->color_selector.g._dst = (SDL_Rect){7 + 15, 100, 14, 8};
	scene->color_selector.b._dst = (SDL_Rect){7 + 30, 100, 14, 8};

	SDLX_Button_Init(&(scene->color_choose), fetch_slider_sprite, 2, (SDL_Rect){10, 137, 7 * 5, 7 * 8}, NULL);
	scene->color_choose.sprite = SDLX_Sprite_Static(ASSETS"color_slider.png");
	scene->color_choose.sprite.dst = SDLX_NULL_SELF;
	scene->color_choose.sprite._dst = (SDL_Rect){10, 137, 7 * 5, 7 * 8};
	scene->color_choose.trigger_box.x -= 4;
	scene->color_choose.trigger_box.w += 8;
	scene->color_choose.trigger_box.h += 8;
	scene->color_choose.trigger_box.y -= 4;

	scene->color_choose.meta = &(scene->color_selector);
	scene->color_choose.meta1 = &(scene->active);
	scene->color_choose.update_fn = button_update_color_selector;

	scene->scale = 8;
	SDLX_Button_Init(&(scene->resolution), fetch_save_sprite, 4, (SDL_Rect){230, 132, 16, 16}, NULL);
	scene->resolution.trigger_fn = button_resolution;
	scene->resolution.meta = &(scene->scale);

	SDLX_Button_Init(&(scene->help), fetch_save_sprite, 5, (SDL_Rect){240, 2, 16, 16}, NULL);
	scene->help.trigger_fn = button_help;

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

	scene = vp_scene;

	SDLX_RenderQueue_Add(NULL, &(scene->color_selector.r));
	SDLX_RenderQueue_Add(NULL, &(scene->color_selector.g));
	SDLX_RenderQueue_Add(NULL, &(scene->color_selector.b));
	update_buttons(scene);

	int i = 0;

	scene->active = NULL;
	SDL_qsort(scene->curves.curves, scene->curves.curve_count, sizeof(*(scene->curves.curves)), clerp_sort);
	while (i < scene->curves.curve_count)
	{
		fetch_slider_sprite(&(scene->curves.curves[i].slider_b.sprite.sprite_data), 0);
		if (scene->curves.curves[i].x == scene->active_id && scene->active_id != -1)
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
	int	temp;
	int	scale = scene->scale;
	SDL_Renderer	*renderer = SDLX_GetDisplay()->renderer;
	SDL_RenderSetScale(renderer, scale, 1);

	while (i < WIN_WIDTH)
	{
		temp = i / scale;
		SDL_SetRenderDrawColor(renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(renderer, temp + 0, 110, temp + 0, 110 + 30);

		SDL_SetRenderDrawColor(renderer, ra[i], ga[i], ba[i], 0);
		SDL_RenderDrawLine(renderer, temp + 0, 110 + 40, temp + 0, 110 + 40 + 30);
		i += scale;
	}
	SDL_RenderSetScale(renderer, 1, 1);

	SDL_Rect	color_box;

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


	if (scene->save_file.isTriggered == SDL_TRUE)
	{
		generate_c_file(scene->curves.curves, scene->curves.curve_count, &(scene->color_start), &(scene->color_end));
		scene->save_file.isTriggered = SDL_FALSE;
	}

	SDL_Rect slider_move_box = {0, 27, WIN_WIDTH, 70};
	if (scene->paste_meta.which == 0 && scene->active != NULL
		&& g_GameInput.GameInput.button_primleft == 1 && g_GameInput_prev.GameInput.button_primleft == 1
		&& SDL_PointInRect(&(g_GameInput.GameInput.primary), &(slider_move_box)) == SDL_TRUE)
		scene->active->slider_b.sprite._dst.x = g_GameInput.GameInput.primary.x - scene->active->slider_b.sprite._dst.w / 2;

	return (NULL);
}
