void	set_color(int val, int *r, int *g, int *b)
{
	int result;

	result = 0;
	if (val < 93)
		result = clerp(0xFF00FF, 0xFFFFFF, val - 0, 93);
	else if (val < 200)
		result = clerp(0xFFFFFF, 0x0000FF, val - 93, 107);
	ctoRGB(result, r, g, b);
}


