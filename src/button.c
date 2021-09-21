/***************************************************************************
 * FILENAME:    button.c
 * DESCRIPTION: Handles the button functionality.
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

#ifdef EMCC
	#include <emscripten.h>
#endif

void	*button_domain_change(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int	*num;

	num = self->meta;
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primleft))
		*num -= 10;

	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primright))
		*num += 10;

	return (NULL);
}

void	*button_slider_change(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int	*min;
	int	*max;
	clerps	**active;
	int	move_to;

	active = self->meta;
	if (*active == NULL)
		return (NULL);

	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft) || g_GameInput.GameInput.button_primright != 0)
	{
		min = self->left;
		max = self->right;

		move_to = (*active)->slider_b.sprite._dst.x + self->meta_length;

		if (- 9 * SLIDER_SCALE < move_to && move_to < WIN_WIDTH / 2 - 9 * SLIDER_SCALE)
			(*active)->slider_b.sprite._dst.x = move_to;
	}

	return (NULL);
}

#ifdef EMCC

EM_JS(char*, paste, (), {
    return Asyncify.handleAsync(async () => {
        document.getElementById("clipping").focus();
        const str = await navigator.clipboard.readText();
        document.getElementById("canvas").focus();
        const size = lengthBytesUTF8(str) + 1;
        const rtn = _malloc(size);
        stringToUTF8(str, rtn, size);
        return rtn;
    });
});

#endif

void	*button_paste(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	char		*paste_text;
	int			color;
	t_clerp_m	*manager;
	int			*id;

	color = 0;
	paste_text = NULL;
	id = button->meta1;
	manager = button->meta;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
	{

#ifdef EMCC
		paste_text = paste();
#else
		paste_text = SDL_GetClipboardText();
#endif

		SDL_Log("Error: %s", SDL_GetError());
		if (paste_text != NULL)
			SDL_Log("Paste is: %s", paste_text);
		if (isxdigit(paste_text[0]))
			SDL_sscanf(paste_text, "%x", &color);
		else
			SDL_sscanf(&(paste_text[1]), "%x", &color);
		SDL_free(paste_text);

		int i = 0;
		while (i < manager->curve_count)
		{
			if (manager->curves[i].x == *id)
				break ;
			i++;
		}
		manager->curves[i].s_color = color;
	}
	return (NULL);
}

void	*button_add_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	t_clerp_m	*manager;
	int			*id;

	manager = button->meta;
	id = button->meta1;

	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
	{
		curve_add(manager, id, 0xFFFFFF);
	}

	return (NULL);
}

void	*button_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int		*active_id;
	clerps	*actual;

	active_id = button->meta;
	actual = (void *)button - offsetof(clerps, slider_b);
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		*(active_id) = actual->x;

	return (NULL);
}

void	*button_generate(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		button->isTriggered = SDL_TRUE;

	return (NULL);
}