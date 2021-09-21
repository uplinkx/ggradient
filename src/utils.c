/***************************************************************************
 * FILENAME:    utils.c
 * DESCRIPTION: Utility functions or the application.
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

int			clerp_sort(const void *ptr1, const void *ptr2)
{
	const clerps	*clerp1;
	const clerps	*clerp2;

	clerp1 = ptr1;
	clerp2 = ptr2;

	if (clerp1->slider_b.sprite._dst.x > clerp2->slider_b.sprite._dst.x)
		return (1);
	else
		return (-1);
}

void		change_message(t_text *text, char *to, SDL_bool allocate)
{
	SDL_Surface	*surface_text;

	if (allocate == SDL_TRUE)
	{
		text->message = SDL_strdup(to);
		text->message_length = SDL_strlen(text->message);
	}
	else
		SDL_strlcpy(text->message, to, text->message_length);

	text->set = text->message;
	TTF_SizeText(text->set_font, text->message, &(text->sprite._dst.w), &(text->sprite._dst.h));
	text->sprite._dst.h *= text->scale;
	text->sprite._dst.w *= text->scale;

	surface_text = TTF_RenderText_Solid(text->set_font, to, text->color);
	SDL_DestroyTexture(text->sprite.sprite_data->texture);
	text->sprite.sprite_data->texture = SDL_CreateTextureFromSurface(SDLX_GetDisplay()->renderer, surface_text);
	SDL_FreeSurface(surface_text);

}

void		create_text(t_text *dst, int color, SDL_Rect loc, char *message, double scale, TTF_Font *font)
{

	dst->color.r = (color >> 24) & 0xFF;
	dst->color.g = (color >> 16) & 0xFF;
	dst->color.b = (color >>  8) & 0xFF;
	dst->color.a = (color >>  0) & 0xFF;

	SDLX_new_Sprite(&(dst->sprite));
	dst->sprite.dst = SDLX_NULL_SELF;
	dst->sprite._dst = loc;
	dst->scale = scale;

	dst->set_font = font;

	dst->sprite.sprite_data = SDL_calloc(1, sizeof(*(dst->sprite.sprite_data)));
	dst->sprite.sprite_data[0].src = NULL;
	dst->sprite.sprite_data[0].cycle = 1;

	dst->sprite.sprite_data->texture = NULL;
	change_message(dst, message, SDL_TRUE);
}

void	update_text(t_text *text, size_t size)
{
	size_t	len;

	len = size;
	if (size == 0)
		len = SDL_strlen(text->set);

	len = SDL_min(len, text->message_length);
	if (SDL_strncmp(text->message, text->set, len) != 0)
	{
		if (len == size) /* meaning the new size is smaller than the allocated size */
			change_message(text, text->set, SDL_FALSE);
		else
		{
			SDL_free(text->message);
			change_message(text, text->set, SDL_TRUE);
		}
	}
}
