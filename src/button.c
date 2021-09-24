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
#include "main_scene.h"

#ifdef EMCC
	#include <emscripten.h>
#endif

#include <stdio.h>

void	*button_domain_change(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int	*num;

	num = self->meta;
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primleft)) { *num -= 10; }
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primright)) { *num += 10; }
	return (NULL);
}

void	*button_resolution(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int	*num;

	num = self->meta;
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primleft)) { *num -= 1; }
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primright)) { *num += 1; }

	*num = SDL_min(18, SDL_max(1, *num));
	return (NULL);
}

void	*button_help(SDL_UNUSED SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	if (SDLX_GAME_RELEASE(g_GameInput, g_GameInput_prev, primleft))
	{
		print_help();
	}
	return (NULL);
}

void	*button_slider_change(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	clerps	**active;
	int	move_to;

	active = self->meta;
	if (*active == NULL)
		return (NULL);

	if (g_GameInput.GameInput.button_primright != 0)
	{
		move_to = (*active)->slider_b.sprite._dst.x + self->meta_length;
		if (- 9 * SLIDER_SCALE < move_to && move_to < WIN_WIDTH / 2 - 9 * SLIDER_SCALE)
			(*active)->slider_b.sprite._dst.x = move_to;
	}

	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft) != 0)
	{
		move_to = (*active)->slider_b.sprite._dst.x + self->meta_length / 5;
		if (- 9 * SLIDER_SCALE < move_to && move_to < WIN_WIDTH / 2 - 9 * SLIDER_SCALE)
			(*active)->slider_b.sprite._dst.x = move_to;
	}

	return (NULL);
}

#ifdef EMCC

EM_JS(char*, js_paste, (), {
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
	clerps		**active;
	t_switcher	*paste_meta;
	size_t		offset;

	color = 0;
	offset = 0;
	paste_text = NULL;
	active = button->meta;
	paste_meta = button->meta1;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
	{

#ifdef EMCC
		paste_text = js_paste();
#else
		paste_text = SDL_GetClipboardText();
#endif

		if (isdigit(paste_text[0]) == SDL_FALSE)
			offset = 1;

		if (paste_meta->which == 1)
			SDL_sscanf(&(paste_text[offset]), "%x", paste_meta->ptr1);
		else if (paste_meta->which == 2)
			SDL_sscanf(&(paste_text[offset]), "%x", paste_meta->ptr2);
		else if (*active != NULL)
			SDL_sscanf(&(paste_text[offset]), "%x", &((*active)->s_color));

		paste_meta->which = 0;

		SDL_free(paste_text);
	}
	return (NULL);
}

void	*button_ends(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int			*which;

	which = button->meta;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		*which = button->meta_length;

	return (NULL);
}

void	*button_update_color_selector(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int			*color;
	t_switcher	*paste_meta;
	t_color_sel	*selector;
	clerps		**active;
	clerps		*t_active;

	active = button->meta1;
	paste_meta = button->meta - offsetof(t_main_scene, color_selector) + offsetof(t_main_scene, paste_meta);
	if (paste_meta->which == 0 && *(active) == NULL)
		return (NULL);

	if (paste_meta->which == 1)
		t_active = button->meta - offsetof(t_main_scene, color_selector) + offsetof(t_main_scene, color_start);
	else if (paste_meta->which == 2)
		t_active = button->meta - offsetof(t_main_scene, color_selector) + offsetof(t_main_scene, color_end);
	else
		t_active = *active;

	active = &(t_active);

	color = &((*active)->s_color);

	int	r, g, b;
	int	temp;

	selector = button->meta;
	if (button->isFocused == SDL_TRUE && g_GameInput.GameInput.button_primleft == 1)
	{
		if (g_GameInput.GameInput.primary.x < 21)
		{
			*color = (*color) & 0x00FFFF;
			temp = g_GameInput.GameInput.primary.y - button->sprite._dst.y;
			temp *= 255.0 / button->sprite._dst.h;
			temp = 255 - temp;
			temp = SDL_min(255, SDL_max(0, temp));
			*color |= ((temp) & 0xFF) << 16;
		}
		else if (g_GameInput.GameInput.primary.x < 36)
		{
			*color = (*color) & 0xFF00FF;
			temp = g_GameInput.GameInput.primary.y - button->sprite._dst.y;
			temp *= 255.0 / button->sprite._dst.h;
			temp = 255 - temp;
			temp = SDL_min(255, SDL_max(0, temp));
			*color |= ((temp) & 0xFF) << 8;
		}
		else if (g_GameInput.GameInput.primary.x < 59)
		{
			*color = (*color) & 0xFFFF00;
			temp = g_GameInput.GameInput.primary.y - button->sprite._dst.y;
			temp *= 255.0 / button->sprite._dst.h;
			temp = 255 - temp;
			temp = SDL_min(255, SDL_max(0, temp));
			*color |= ((temp) & 0xFF) << 0;
		}
	}
	ctoRGB(*color, &r, &g, &b);
	selector->r._dst.y = 135 + 7 * 8 - (r * (button->sprite._dst.h / 255.0));
	selector->g._dst.y = 135 + 7 * 8 - (g * (button->sprite._dst.h / 255.0));
	selector->b._dst.y = 135 + 7 * 8 - (b * (button->sprite._dst.h / 255.0));



	return (NULL);
}

void	*button_add_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	t_clerp_m	*manager;
	int			*id;

	id = button->meta1;
	manager = button->meta;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		curve_add(manager, id, 0xFFFFFF);

	return (NULL);
}

void	*button_remove_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	t_clerp_m	*manager;
	int			*id;

	id = button->meta1;
	manager = button->meta;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		curve_remove(manager, id);

	return (NULL);
}

void	*button_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	int			*active_id;
	t_switcher	*paste_meta;
	clerps		*actual;

	active_id = button->meta;
	/* We have pointers that are addresses of a member in the struct. That means that we can jump to
	other members if we knew the offsets of where the other members are because they are relative
	to each other. This easily knowable using offsetof, or the __builtin_offsetof macros. */
	actual = (void *)button - offsetof(clerps, slider_b);
	paste_meta = (void *)active_id - offsetof(t_main_scene, active_id) + offsetof(t_main_scene, paste_meta);
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
	{
		*(active_id) = actual->x;
		paste_meta->which = 0;
	}

	return (NULL);
}

void	*button_generate(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
		button->isTriggered = SDL_TRUE;

	return (NULL);
}

void	*button_view_output(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length)
{
	char	**string_ptr;
	SDL_bool	*popup_isDisabled;

	string_ptr = button->meta;
	popup_isDisabled = button->meta1;
	if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
	{
		*popup_isDisabled = SDL_FALSE;
		button->isTriggered = SDL_TRUE;
		SDL_Log("[Saved]: %s", *string_ptr);
	}

	return (NULL);
}