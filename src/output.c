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

void	generate_c_file(clerps *curves, int curve_count, clerps *start, clerps *end, int min, int max)
{
	int			i;
	int			curr;
	char		buffer[1000];
	int			offset;
	const char	*file_name;
	SDL_RWops	*file;

	file_name = "clerp_control_flow.c";

	file = SDL_RWFromFile(file_name, "w");
	SDL_Delay(100);

	curr = 0;
	curr += SDL_snprintf(buffer, sizeof(buffer), "void	set_color(int val, int *r, int *g, int *b)\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "{\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	int result;\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	result = 0;\n");

	i = 0;
	offset = min;
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	if (val < %d)\n", curves[0].slider_b.sprite._dst.x);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", start->s_color, curves[0].s_color, start->slider_b.sprite._dst.x, curves[0].slider_b.sprite._dst.x);
	SDL_RWwrite(file, buffer, sizeof(char), curr);
	while (i < curve_count - 1)
	{
		curr = 0;
		curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	else if (val < %d)\n", curves[i + 1].slider_b.sprite._dst.x);
		curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", curves[i].s_color, curves[i + 1].s_color, curves[i].slider_b.sprite._dst.x, curves[i + 1].slider_b.sprite._dst.x - curves[i].slider_b.sprite._dst.x);
		SDL_RWwrite(file, buffer, sizeof(char), curr);
		offset += curves[i].slider_b.sprite._dst.x * DISPLAY_SCALE;
		i++;
	}
	curr = 0;
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	else if (val < %d)\n", WIN_WIDTH / DISPLAY_SCALE);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "		result = clerp(0x%.6X, 0x%.6X, val - %d, %d);\n", curves[i].s_color, end->s_color, curves[i].slider_b.sprite._dst.x, max / DISPLAY_SCALE - curves[i].slider_b.sprite._dst.x);
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "	ctoRGB(result, r, g, b);\n");
	curr += SDL_snprintf(buffer + curr, sizeof(buffer) - curr, "}\n");
	SDL_RWwrite(file, buffer, sizeof(char), curr);
	SDL_RWclose(file);
	(void)max;
}

