/***************************************************************************
 * FILENAME:    scene_utils.c
 * DESCRIPTION: Utility functions to have a clean main loop.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 21Sep2021
***************************************************************************/

#include "main.h"
#include "main_scene.h"

void	update_buttons(t_main_scene *scene)
{
	SDLX_Button_Update(&(scene->popup));

	SDLX_Button_Update(&(scene->increment));
	SDLX_Button_Update(&(scene->decrement));
	SDLX_Button_Update(&(scene->slider_inc));
	SDLX_Button_Update(&(scene->slider_dec));
	SDLX_Button_Update(&(scene->sliders_start));
	SDLX_Button_Update(&(scene->sliders_end));
	SDLX_Button_Update(&(scene->paste));
	SDLX_Button_Update(&(scene->save_file));
	SDLX_Button_Update(&(scene->view_file));

	SDLX_Button_Update(&(scene->add));

}

void	draw_slider_color(int color, SDL_Rect *color_rect, int x, SDL_Renderer *renderer)
{
	int	r, g, b;

	color_rect->x = x * 2 + 2;
	ctoRGB(color, &r, &g, &b);
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderFillRect(renderer, color_rect);
}