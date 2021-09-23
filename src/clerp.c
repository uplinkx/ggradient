/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clerp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: home <home@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 03:20:28 by home              #+#    #+#             */
/*   Updated: 2021/09/22 20:04:12 by home             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	ctoRGB(int color, int *r, int *g, int *b)
{
	*r = (color >> 16) & 0xFF;
	*g = (color >>  8) & 0xFF;
	*b = (color >>  0) & 0xFF;
}

/**
* \brief Linear interpolates between min and max
* \param min the min value
* \param max the max value
* \param t propotion of interpolation, value between 0 and tmax
* \param tmax the max value t can take
*
* \brief
* Does not handle overflow or underflows well. BEWARE!
*/
integer	lerp(integer min, integer max, int t, int tmax)
{
	return (min + ((max - min) * t) / tmax);
}

int		clerp(int color1, int color2, int t, int tmax)
{
	int	r1, g1, b1;
	int	r2, g2, b2;

	int	r_res, g_res, b_res;
	int result;

	ctoRGB(color1, &r1, &g1, &b1);
	ctoRGB(color2, &r2, &g2, &b2);

	r_res = lerp(r1, r2, t, tmax);
	g_res = lerp(g1, g2, t, tmax);
	b_res = lerp(b1, b2, t, tmax);

	result = (r_res << 16) |  (g_res <<  8) | (b_res <<  0);
	return (result);
}

void	grad_lerp(int *colors, int start, int end, int tmax, int offset)
{
	int	i;

	i = 0;
	while (i < tmax)
	{
		colors[i + offset] = clerp(start, end, i, tmax);
		i++;
	}
}

void	create_gradient_ends(int *dest, clerps color_start, clerps color_end)
{
	int	start_color;
	int	end_color;

	start_color = color_start.s_color;
	end_color = color_end.s_color;
	grad_lerp(dest, start_color, end_color, WIN_WIDTH, 0);
}

void	create_gradient(int *dest, int curve_amount, clerps *lerp_info, clerps color_start, clerps color_end)
{
	int	i;
	int	start_color;
	int	end_color;
	int	offset;
	int	distance;
	int	bias;

	if (curve_amount <= 0)
	{
		create_gradient_ends(dest, color_start, color_end);
		return ;
	}

	i = 0;
	offset = 0;
	bias = 9 / 2.0 * SLIDER_SCALE + 2;

	start_color = color_start.s_color;
	end_color = lerp_info[0].s_color;
	distance = (lerp_info[0].slider_b.sprite._dst.x - color_start.slider_b.sprite._dst.x) * 2 + bias + bias + 7;
	grad_lerp(dest, start_color, end_color, distance, offset);

	offset += distance;
	while (i < curve_amount - 1)
	{
		start_color = end_color;
		end_color = lerp_info[i + 1].s_color;
		distance = (lerp_info[i + 1].slider_b.sprite._dst.x - lerp_info[i].slider_b.sprite._dst.x) * 2;
		grad_lerp(dest, start_color, end_color, distance, offset);

		offset += distance;
		i++;
	}

	start_color = end_color;
	end_color = color_end.s_color;
	distance = (WIN_WIDTH / 2 - lerp_info[i].slider_b.sprite._dst.x) * 2;
	grad_lerp(dest, start_color, end_color, distance, offset);
}
