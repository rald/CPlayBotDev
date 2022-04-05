#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"

#include <SDL2/SDL.h>
#include <math.h>

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);

void DrawCircle(SDL_Renderer *renderer, int x0, int y0, int r);

void FillCircle(SDL_Renderer *renderer, int x, int y, int r);

#ifdef GRAPHICS_IMPLEMENTATION

static int sgn(int n)
{
	return n < 0 ? -1 : n > 0 ? 1 : 0;
}

static int clamp(int d, int min, int max)
{
	const double t = d < min ? min : d;
	return t > max ? max : t;
}

static double ConstrainX(double x)
{
	x = fmod(x, SCREEN_WIDTH);
	if (x < 0)
		x += SCREEN_WIDTH;
	return x;
}

static double ConstrainY(double y)
{
	y = fmod(y, SCREEN_HEIGHT);
	if (y < 0)
		y += SCREEN_HEIGHT;
	return y;
}

void DrawPoint(SDL_Renderer *renderer, int x, int y)
{
	x = ConstrainX(x);
	y = ConstrainY(y);
	SDL_RenderDrawPoint(renderer, x, y);
}

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx = x2 - x1;
	dy = y2 - y1;
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	DrawPoint(renderer, px, py);

	if (dxabs >= dyabs)
	{
		for (i = 0; i < dxabs; i++)
		{
			y += dyabs;
			if (y >= dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			DrawPoint(renderer, px, py);
		}
	}
	else
	{
		for (i = 0; i < dyabs; i++)
		{
			x += dxabs;
			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			DrawPoint(renderer, px, py);
		}
	}
}

void DrawCircle(SDL_Renderer *renderer, int x0, int y0, int r)
{
	int f = 1 - r;
	int ddF_x = 0;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	DrawPoint(renderer, x0, y0 + r);
	DrawPoint(renderer, x0, y0 - r);
	DrawPoint(renderer, x0 + r, y0);
	DrawPoint(renderer, x0 - r, y0);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;

			x++;
			ddF_x += 2;
			f += ddF_x + 1;
			DrawPoint(renderer, x0 + x, y0 + y);
			DrawPoint(renderer, x0 - x, y0 + y);
			DrawPoint(renderer, x0 + x, y0 - y);
			DrawPoint(renderer, x0 - x, y0 - y);
			DrawPoint(renderer, x0 + y, y0 + x);
			DrawPoint(renderer, x0 - y, y0 + x);
			DrawPoint(renderer, x0 + y, y0 - x);
			DrawPoint(renderer, x0 - y, y0 - x);
		}
	}
}

void FillCircle(SDL_Renderer *renderer, int x, int y, int r)
{
	int r2 = r * r;
	int area = r2 << 2;
	int rr = r << 1;

	for (int i = 0; i < area; i++)
	{
		int tx = (i % rr) - r;
		int ty = (i / rr) - r;

		if (tx * tx + ty * ty <= r2)
			DrawPoint(renderer, x + tx, y + ty);
	}
}

#endif /* GRAPHICS_IMPLEMETATION */

#endif /* GRAPHICS_H */
