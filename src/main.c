/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: home <home@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 02:31:10 by home              #+#    #+#             */
/*   Updated: 2021/09/20 08:26:52 by home             ###   ########.fr       */
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

int	main(void)
{
	// int			ticks;
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
