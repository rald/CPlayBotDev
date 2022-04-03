#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#include <math.h>

#include "common.h"

double sgn(double n);
double clamp(double d, double min, double max);

double ComputeDistance(double x1, double y1, double x2, double y2);

double ConstrainX(double x);
double ConstrainY(double y);
double ConstrainAngle(double a);

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);

void DrawCircle(SDL_Renderer *renderer, int xc, int yc, int r);

#ifdef GRAPHICS_IMPLEMENTATION

double ComputeDistance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

double sgn(double n)
{
	return n < 0 ? -1 : n > 0 ? 1 : 0;
}

double clamp(double d, double min, double max)
{
	double t = d < min ? min : d;
	return t > max ? max : t;
}

double ConstrainX(double x)
{
	x = fmod(x, SCREEN_WIDTH);
	if (x < 0)
		x += SCREEN_WIDTH;
	return x;
}

double ConstrainY(double y)
{
	y = fmod(y, SCREEN_HEIGHT);
	if (y < 0)
		y += SCREEN_HEIGHT;
	return y;
}

double ConstrainAngle(double a)
{
	a = fmod(a, 360);
	if (a < 0)
		a += 360;
	return a;
}

void DrawPoint(SDL_Renderer *renderer, int x, int y)
{
	SDL_RenderDrawPoint(renderer, ConstrainX(x), ConstrainY(y));
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

static void circle(SDL_Renderer *renderer, int xc, int yc, int x, int y)
{
	DrawPoint(renderer, xc + x, yc + y);
	DrawPoint(renderer, xc - x, yc + y);
	DrawPoint(renderer, xc + x, yc - y);
	DrawPoint(renderer, xc - x, yc - y);
	DrawPoint(renderer, xc + y, yc + x);
	DrawPoint(renderer, xc - y, yc + x);
	DrawPoint(renderer, xc + y, yc - x);
	DrawPoint(renderer, xc - y, yc - x);
}

void DrawCircle(SDL_Renderer *renderer, int xc, int yc, int r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;

	circle(renderer, xc, yc, x, y);

	while (y >= x)
	{
		x++;

		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;

		circle(renderer, xc, yc, x, y);
	}
}

#endif /* GRAPHICS_IMPLEMETATION */

#endif /* GRAPHICS_H */
