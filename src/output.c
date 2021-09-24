/***************************************************************************
 * FILENAME:    output.c
 * DESCRIPTION: Functions responsible for generating the C file.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 16Sep2021
***************************************************************************/

#include "main.h"

/*
** The function will fail if sizeof(buffer) - curr is ever negative.
** What makes this difficult to fix is that this parameter is passed to
** SDL_snprintf and is converted to size_t which is unsigned. Meaning that
** SDL_snprintf will not catch this as an error and see it as a big size.
**
** The easy fix would be to do an if check after each SDL_snprintf,
** I choose not to do this because it would just make the code messier
** and I could simply `assume` that this would never be the case with a buffer
** sizeof(buffer) big.
*/

#ifdef EMCC
# define GRAD_STRING_ROUTINE (1)
#else
# define GRAD_FILE_ROUTINE (1)
# define GRAD_STRING_ROUTINE (1)
#endif

#include <stdio.h>

void	generate_c_file(clerps *curves, int curve_count, clerps *start, clerps *end)
{
	int			i;
	int			curr;
	char		buffer[10000];
	int			offset;

// #ifdef GRAD_STRING_ROUTINE
// 	SDL_free(*s);
// #endif

#ifdef GRAD_FILE_ROUTINE
	const char	*file_name;
	SDL_RWops	*file;

	file_name = "clerp_control_flow.c";

	file = SDL_RWFromFile(file_name, "w");
	SDL_Delay(100);
#endif


	curr = 0;
	curr += SDL_snprintf(buffer, sizeof(buffer), 					"void	set_color(int val, int *r, int *g, int *b)\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"{\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"	int result;\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"	result = 0;\n");

	i = 0;
	offset = 0;
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	if (val < %d)\n", curves[0].slider_b.sprite._dst.x);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", start->s_color, curves[0].s_color, start->slider_b.sprite._dst.x, curves[0].slider_b.sprite._dst.x);
	while (i < curve_count - 1)
	{
		curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,	"	else if (val < %d)\n", curves[i + 1].slider_b.sprite._dst.x);
		curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,	"		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", curves[i].s_color, curves[i + 1].s_color, curves[i].slider_b.sprite._dst.x, curves[i + 1].slider_b.sprite._dst.x - curves[i].slider_b.sprite._dst.x);
		offset += curves[i].slider_b.sprite._dst.x * DISPLAY_SCALE;
		i++;
	}
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"	else if (val < %d)\n", WIN_WIDTH / DISPLAY_SCALE);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", curves[i].s_color, end->s_color, curves[i].slider_b.sprite._dst.x, WIN_WIDTH / DISPLAY_SCALE - curves[i].slider_b.sprite._dst.x);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"	ctoRGB(result, r, g, b);\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr,		"}\n\n\n");

#ifdef GRAD_STRING_ROUTINE
	// *s = SDL_strdup(buffer);
	printf("=================[GENERATED FILE]=================\n\n%s", buffer);
#endif

#ifdef GRAD_FILE_ROUTINE
	SDL_RWwrite(file, buffer, sizeof(char), curr);
	SDL_RWclose(file);
#endif

}

