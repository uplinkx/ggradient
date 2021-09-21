/***************************************************************************
 * FILENAME:    fetch_button_sprite.c
 * DESCRIPTION: Get the button sprites from the png file.
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

SDLX_Sprite_Data *carve_button_sprite(void)
{
	size_t				i;
	SDL_Texture			*texture;
	SDLX_Sprite_Data	*result;

	result = SDL_calloc(2, sizeof(*result));
	texture = SDLX_LoadTexture(ASSETS"button.png");

	i = 0;

	/* The Button */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 0, 48, 48};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;
	/* The Button Hover */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){48, 0, 48, 48};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	return (result);
}

int		fetch_button_sprite(SDLX_Sprite_Data **dst, int no)
{
	static SDLX_Sprite_Data	*sprite_arr;

	if (sprite_arr == NULL)
		sprite_arr = carve_button_sprite();

	if (no == 0) { *dst = &(sprite_arr[0]); }
	else if (no == 1) { *dst = &(sprite_arr[1]); }

	return (EXIT_SUCCESS);
}

SDLX_Sprite_Data *carve_add_sprite(void)
{
	size_t				i;
	SDL_Texture			*texture;
	SDLX_Sprite_Data	*result;

	result = SDL_calloc(1, sizeof(*result));
	texture = SDLX_LoadTexture(ASSETS"add.png");

	i = 0;

	/* The Button */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 0, 16, 16};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	return (result);
}

int		fetch_add_sprite(SDLX_Sprite_Data **dst, int no)
{
	static SDLX_Sprite_Data	*sprite_arr;

	if (sprite_arr == NULL)
		sprite_arr = carve_add_sprite();

	if (no == 0) { *dst = &(sprite_arr[0]); }

	return (EXIT_SUCCESS);
}

SDLX_Sprite_Data *carve_slider_sprite(void)
{
	size_t				i;
	SDL_Texture			*texture;
	SDLX_Sprite_Data	*result;

	result = SDL_calloc(2, sizeof(*result));
	texture = SDLX_LoadTexture(ASSETS"slider.png");

	i = 0;

	/* Slider */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 0, 18, 37};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	/* active Slider */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){18, 0, 18, 37};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	return (result);
}

int		fetch_slider_sprite(SDLX_Sprite_Data **dst, int no)
{
	static SDLX_Sprite_Data	*sprite_arr;

	if (sprite_arr == NULL)
		sprite_arr = carve_slider_sprite();

	if (no == 0) { *dst = &(sprite_arr[0]); }
	if (no == 1) { *dst = &(sprite_arr[1]); }

	return (EXIT_SUCCESS);
}

SDLX_Sprite_Data *carve_slider_move_sprite(void)
{
	size_t				i;
	SDL_Texture			*texture;
	SDLX_Sprite_Data	*result;

	result = SDL_calloc(4, sizeof(*result));
	texture = SDLX_LoadTexture(ASSETS"slider_move.png");

	i = 0;

	/* Slider_move */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 0, 16, 16};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	/* active Slider_move */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){16, 0, 16, 16};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;
	/* Slider_move */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 16, 16, 16};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	/* active Slider_move */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){16, 16, 16, 16};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	return (result);
}

int		fetch_slider_move_sprite(SDLX_Sprite_Data **dst, int no)
{
	static SDLX_Sprite_Data	*sprite_arr;

	if (sprite_arr == NULL)
		sprite_arr = carve_slider_move_sprite();

	if (no == 0) { *dst = &(sprite_arr[0]); }
	if (no == 1) { *dst = &(sprite_arr[1]); }
	if (no == 2) { *dst = &(sprite_arr[2]); }
	if (no == 3) { *dst = &(sprite_arr[3]); }

	return (EXIT_SUCCESS);
}

SDLX_Sprite_Data *carve_save_sprite(void)
{
	size_t				i;
	SDL_Texture			*texture;
	SDLX_Sprite_Data	*result;

	result = SDL_calloc(2, sizeof(*result));
	texture = SDLX_LoadTexture(ASSETS"save.png");

	i = 0;

	/* Save */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){0, 0, 18, 18};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	/* Save Hovered */
	result[i].texture = texture;
	result[i]._src = (SDL_Rect){18, 0, 18, 18};
	result[i].src = &(result[i]._src);
	result[i].cycle = 1;
	i++;

	return (result);
}

int		fetch_save_sprite(SDLX_Sprite_Data **dst, int no)
{
	static SDLX_Sprite_Data	*sprite_arr;

	if (sprite_arr == NULL)
		sprite_arr = carve_save_sprite();

	if (no == 0) { *dst = &(sprite_arr[0]); }
	if (no == 1) { *dst = &(sprite_arr[1]); }

	return (EXIT_SUCCESS);
}