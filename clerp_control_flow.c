void	set_color(int val, int *r, int *g, int *b)
{
	int result;

	result = 0;
	if (val < 90)
		result = clerp(0xFF00FF, 0xFFFFFF, val - 0, 90);
	else if (val < 200)
		result = clerp(0xFFFFFF, 0x0000FF, val - 90, 110);
	ctoRGB(result, r, g, b);
}
