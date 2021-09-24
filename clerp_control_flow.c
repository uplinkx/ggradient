void	set_color(int val, int *r, int *g, int *b)
{
	int result;

	result = 0;
	if (val < 118)
		result = clerp(0x3B3638, 0x000000, val - 0, 118);
	else if (val < 256)
		result = clerp(0x000000, 0xFFFFFF, val - 118, 138);
	ctoRGB(result, r, g, b);
}
