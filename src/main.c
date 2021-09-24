/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: home <home@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 02:31:10 by home              #+#    #+#             */
/*   Updated: 2021/09/23 19:32:06 by home             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#ifdef EMCC
	#include <emscripten.h>
#endif

void	context_init(t_context *context)
{
	context->shouldQuit = SDL_FALSE;
	context->shouldChange = SDL_TRUE;

	context->meta = NULL;

	context->init_fn = main_scene_init;

	TTF_Init();
	context->font = TTF_OpenFont(ASSETS"chary___.ttf", 150);
}

void	main_loop(void *context_addr)
{
	t_context	*context;
	SDL_Color	bg_color = {59, 54, 56, 0};

	context = context_addr;
	if (context->shouldChange == SDL_TRUE)
	{
		context->init_fn(context, context->meta);
		context->shouldChange = SDL_FALSE;
	}

	context->shouldQuit = SDLX_poll();
	SDLX_record_input(NULL);
	SDLX_GameInput_Mouse_Fill(&(g_GameInput), SDL_TRUE);

	context->update_fn(context, context->meta);

	if (context->shouldQuit != SDL_TRUE && SDLX_discrete_frames(NULL) != EXIT_FAILURE)
	{
		SDLX_RenderQueue_Flush(NULL, NULL, SDL_TRUE);
		SDLX_ScreenReset(SDLX_GetDisplay()->renderer, &(bg_color));
	}

	if (context->shouldChange == SDL_TRUE)
	{
		SDLX_CollisionBucket_Flush(NULL);
		SDLX_RenderQueue_Flush(NULL, SDLX_GetDisplay()->renderer, SDL_FALSE);

		context->close_fn(context, context->meta);
	}
}

int	main(void)
{
	t_context	context;

	SDLX_GetDisplay();
	context_init(&(context));

	#ifdef EMCC
		emscripten_set_main_loop_arg(main_loop, (void *)&(context), 0, SDL_TRUE);
	#else
		while (context.shouldQuit == SDL_FALSE)
			main_loop(&(context));
	#endif

	return (EXIT_SUCCESS);
}
