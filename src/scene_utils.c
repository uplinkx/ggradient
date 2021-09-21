/***************************************************************************
 * FILENAME:    scene_utils.c
 * DESCRIPTION: Utility functions for scenes.
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

void	*new_scene(size_t size, t_context *context, char *background_path, void *(close)(t_context *, void *), void *(update)(t_context *, void *))
{
	void *result;

	result = SDL_calloc(1, size);

	SDL_assert(close != NULL);
	SDL_assert(update != NULL);

	context->close_fn = close;
	context->update_fn = update;
	context->meta = result;
	context->shouldChange = SDL_FALSE;

	if (background_path != NULL)
	{
		context->background = SDLX_Sprite_Static(background_path);
		SDLX_SetBackground(&(context->background));
	}
	else
		SDLX_SetBackground(NULL);

	return (result);
}