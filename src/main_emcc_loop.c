/***************************************************************************
 * FILENAME:    main_emcc_loop.c
 * DESCRIPTION: main loop that emmc needs, and thus the main app loop.
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

void	main_loop(void *context_addr)
{
	t_context	*context;

	context = context_addr;
	if (context->shouldChange == SDL_TRUE)
	{
		context->init_fn(context, context->meta);
		context->shouldChange = SDL_FALSE;
	}

	context->shouldQuit = SDLX_poll();
	SDLX_record_input(NULL);
	// SDLX_KeyMap(&(g_GameInput.key_mapper), g_GameInput.keystate);
	SDLX_GameInput_Mouse_Fill(&(g_GameInput), SDL_TRUE);

	context->update_fn(context, context->meta);

	if (context->shouldQuit != SDL_TRUE && SDLX_discrete_frames(NULL) != EXIT_FAILURE)
	{
		SDLX_RenderQueue_Flush(NULL, NULL, SDL_TRUE);
		SDLX_ScreenReset(SDLX_GetDisplay()->renderer, NULL);
	}

	if (context->shouldChange == SDL_TRUE)
	{
		SDLX_CollisionBucket_Flush(NULL);
		SDLX_RenderQueue_Flush(NULL, SDLX_GetDisplay()->renderer, SDL_FALSE);

		context->close_fn(context, context->meta);
	}
}