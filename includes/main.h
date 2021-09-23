/***************************************************************************
 * FILENAME:    main.h
 * DESCRIPTION: Header Files for Gradient Application.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 16Sep2021
***************************************************************************/



#ifndef main_H
# define main_H

# include <SDL2/SDL_image.h>

# include "SDLX/SDLX.h"

# include "main_structs.h"

# define ASSETS "assets/"

# define SLIDER_SCALE (1.1)

void	main_loop(void *context_addr);

void	*new_scene(size_t size, t_context *context, char *background_path, void *(close)(t_context *, void *), void *(update)(t_context *, void *));

void	*main_scene_init(t_context *context, void *vp_scene);
void	*main_scene_close(t_context *context, void *vp_scene);
void	*main_scene_update(t_context *context, void *vp_scene);

int		fetch_add_sprite(SDLX_Sprite_Data **dst, int no);
int		fetch_button_sprite(SDLX_Sprite_Data **dst, int no);
int		fetch_slider_sprite(SDLX_Sprite_Data **dst, int no);
int		fetch_slider_move_sprite(SDLX_Sprite_Data **dst, int no);
int		fetch_save_sprite(SDLX_Sprite_Data **dst, int no);

void	*button_domain_change(SDLX_button *self, void *meta, SDL_UNUSED size_t length);
void	*button_slider_change(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_paste(SDL_UNUSED SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_add_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_remove_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_slider(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_generate(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);
void	*button_view_output(SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t length);

void	update_text(t_text *text, size_t size);
void	update_text_wrapped(t_text *text, size_t size, Uint32 width);
void	create_text(t_text *dst, int color, SDL_Rect loc, char *message, double scale, TTF_Font *font);

int		clerp(int color1, int color2, int t, int tmax);
void	ctoRGB(int color, int *r, int *g, int *b);
void	create_gradient(int *dest, int curve_amount, clerps *lerp_info, clerps color_start, clerps color_end);

void	curve_add(t_clerp_m *manager, int *active, int color);
void	curve_remove(t_clerp_m *manager, int *active_id);
int		clerp_sort(const void *ptr1, const void *ptr2);
void	curves_init(t_clerp_m *manager);

void	generate_c_file(clerps *curves, int curve_count, clerps *start, clerps *end, int min, int max);

#endif
