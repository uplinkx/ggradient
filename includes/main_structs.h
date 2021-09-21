/***************************************************************************
 * FILENAME:    main_structs.h
 * DESCRIPTION: The main structs for the Gradient program.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 13Sep2021
***************************************************************************/

#ifndef MAIN_STRUCTS_H
# define MAIN_STRUCTS_H

#include "SDLX/SDLX.h"

struct s_context;

typedef int integer;
typedef void *(t_scene_fn)(struct s_context *, void *);

typedef struct	clerps
{
	int			s_color;

	// SDLX_Sprite	slider;
	SDLX_button	slider_b;
	int			x;
}				clerps;

typedef struct	s_clerp_m
{
	int			capacity;
	int			curve_count;
	int			id_at;

	clerps		*curves;
}				t_clerp_m;

typedef struct	s_context
{
	SDL_bool	shouldQuit;
	SDL_bool	shouldChange;

	void		*meta;

	SDLX_Sprite	background;
	SDL_Texture	*capture_texture;

	t_scene_fn	*init_fn;
	t_scene_fn	*update_fn;
	t_scene_fn	*close_fn;

	TTF_Font	*font;
}				t_context;

typedef struct	s_text
{
	SDL_Color	color;
	double		scale;
	size_t		message_length;
	char		*message;
	char		*set;
	SDLX_Sprite	sprite;

	TTF_Font	*set_font;
}				t_text;

#endif