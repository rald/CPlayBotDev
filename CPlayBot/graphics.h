#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <math.h>

#include "common.h"

double sgn(double n);
double clamp(double d, double min, double max);

bool incircle(double x, double y, double cx, double cy, double cr);
bool inrect(double x, double y, double rx, double ry, double rw, double rh);

double distance(double x1, double y1, double x2, double y2);

double WrapX(double x);
double WrapY(double y);
double WrapAngle(double a);

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
void DrawCircle(SDL_Renderer *renderer, int xc, int yc, int r);
void DrawPoly(SDL_Renderer *renderer, double *pointsx, double *pointsy, int npoints);

#ifdef GRAPHICS_IMPLEMENTATION

double distance(double x1, double y1, double x2, double y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

double sgn(double n) {
	return n < 0 ? -1 : n > 0 ? 1 : 0;
}

double clamp(double d, double min, double max) {
	double t = d < min ? min : d;
	return t > max ? max : t;
}

bool incirc(double x, double y, double cx, double cy, double cr) {
	double dx = cx - x;
	double dy = cy - y;
	return dx * dx + dy * dy < cr * cr;
}

bool inrect(double x, double y, double rx, double ry, double rw, double rh) {
	return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}

double WrapX(double x) {
	/*
		x = fmod(x, SCREEN_WIDTH);
		if (x < 0)
			x += SCREEN_WIDTH;
		return x;
	*/
	return clamp(x, 0, SCREEN_WIDTH);
}

double WrapY(double y) {
	/*
		y = fmod(y, SCREEN_HEIGHT);
		if (y < 0)
			y += SCREEN_HEIGHT;
		return y;
	*/
	return clamp(y, 0, SCREEN_HEIGHT);
}

double WrapAngle(double a) {
	a = fmod(a * RAD2DEG, 360);
	if (a < 0)
		a += 360;
	return a * DEG2RAD;
}

void DrawPoint(SDL_Renderer *renderer, int x, int y) {
	if(x >= 0 || x < SCREEN_WIDTH || y >= 0 | y < SCREEN_HEIGHT) {
		SDL_RenderDrawPoint(renderer, x, y);
	}
}

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
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

	if (dxabs >= dyabs) {
		for (i = 0; i < dxabs; i++) {
			y += dyabs;
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			DrawPoint(renderer, px, py);
		}
	} else {
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			DrawPoint(renderer, px, py);
		}
	}
}

static void circle(SDL_Renderer *renderer, int xc, int yc, int x, int y) {
	DrawPoint(renderer, xc + x, yc + y);
	DrawPoint(renderer, xc - x, yc + y);
	DrawPoint(renderer, xc + x, yc - y);
	DrawPoint(renderer, xc - x, yc - y);
	DrawPoint(renderer, xc + y, yc + x);
	DrawPoint(renderer, xc - y, yc + x);
	DrawPoint(renderer, xc + y, yc - x);
	DrawPoint(renderer, xc - y, yc - x);
}

void DrawCircle(SDL_Renderer *renderer, int xc, int yc, int r) {
	int x = 0, y = r;
	int d = 3 - 2 * r;

	circle(renderer, xc, yc, x, y);

	while (y >= x) {
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else
			d = d + 4 * x + 6;

		circle(renderer, xc, yc, x, y);
	}
}

void DrawPoly(SDL_Renderer *renderer, double *pointsx, double *pointsy, int npoints) {
	for(int i = 0; i < npoints - 1; i++) {
		DrawLine(renderer, pointsx[i], pointsy[i], pointsx[i + 1], pointsy[i + 1]);
	}
	DrawLine(renderer, pointsx[npoints - 1], pointsy[npoints - 1], pointsx[0], pointsy[0]);
}

#endif /* GRAPHICS_IMPLEMETATION */

#endif /* GRAPHICS_H */
