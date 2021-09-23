void	set_color(int val, int *r, int *g, int *b)
{
	int result;

	result = 0;
	if (val < 90)
		result = clerp(0xFF00FF, 0xFC039D, val - 0, 90);
	else if (val < 200)
		result = clerp(0xFC039D, 0x0000FF, val - 90, 110);
	ctoRGB(result, r, g, b);
}


