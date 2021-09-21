/***************************************************************************
 * FILENAME:    bar.c
 * DESCRIPTION: Draws the bar.
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

void	draw_bar(int *bar_c, int start, int end)
{
	int	i;
	int	stop;

	i = 0;
	stop = (end - start) * DISPLAY_SCALE;
	while (i < stop)
	{
		bar_c[i] = clerp(0x0, 0xFFFFFF, i, stop);
		i++;
	}
}

