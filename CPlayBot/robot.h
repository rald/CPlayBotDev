#ifndef ROBOT_H
#define ROB0T_H

#include <stdbool.h>
#include <math.h>

#include "common.h"

#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

#define DEG2RAD (M_PI / 180.0)

typedef struct Robot Robot;

struct Robot
{
	int id;
	double x, y;
	double heading;
};

typedef struct Scan Scan;

struct Scan
{
	double angle;
	double distance;
};

Robot *Robot_Create(int id, double x, double y, double h);
void Robot_Destroy(Robot *robot);
void Robot_Draw(SDL_Renderer *renderer, Robot *robot);
void Robot_Turn(Robot *robot, double a);
void Robot_Scan(SDL_Renderer *renderer, Robot *robot, Robot **robots, int nrobots, double d, double w);

#ifdef ROBOT_IMPLEMENTATION

Robot *Robot_Create(int id, double x, double y, double heading)
{
	Robot *robot = malloc(sizeof(*robot));
	if (robot)
	{
		robot->id = id;
		robot->x = x;
		robot->y = y;
		robot->heading = ConstrainAngle(heading);
	}
	return robot;
}

void Robot_Destroy(Robot *robot)
{
	free(robot);
	robot = NULL;
}

void Robot_Draw(SDL_Renderer *renderer, Robot *robot)
{
	int n = 4;
	double x[] = {0, -1, 2, -1};
	double y[] = {0, 1, 0, -1};
	double sz = 8;
	double nx[n];
	double ny[n];

	for (int i = 0; i < n; i++)
	{
		x[i] *= sz;
		y[i] *= sz;
	}

	for (int i = 0; i < n; i++)
	{
		nx[i] = x[i] * cos(robot->heading * DEG2RAD) - y[i] * sin(robot->heading * DEG2RAD);
		ny[i] = x[i] * sin(robot->heading * DEG2RAD) + y[i] * cos(robot->heading * DEG2RAD);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i < n - 1; i++)
	{
		DrawLine(renderer, nx[i] + robot->x, ny[i] + robot->y, nx[i + 1] + robot->x, ny[i + 1] + robot->y);
	}
	DrawLine(renderer, nx[n - 1] + robot->x, ny[n - 1] + robot->y, nx[0] + robot->x, ny[0] + robot->y);
}

void Robot_Move(Robot *robot, double distance)
{
	robot->x = ConstrainX(distance * cos(robot->heading * DEG2RAD) + robot->x);
	robot->y = ConstrainY(distance * sin(robot->heading * DEG2RAD) + robot->y);
}

void Robot_Turn(Robot *robot, double angle)
{
	robot->heading = ConstrainAngle(robot->heading + angle);
}

static bool insector(Robot *r1, Robot *r2, double d, double w)
{
	double sa = (r1->heading - w / 2) * DEG2RAD;
	double ea = (r1->heading + w / 2) * DEG2RAD;

	if (sa > ea)
	{
		double t = sa;
		sa = ea;
		ea = t;
	}

	double r = ComputeDistance(r1->x, r1->y, r2->x, r2->y);
	double a = atan2(r2->y - r1->y, r2->x - r1->x);

	return a >= sa && a <= ea && r < d;
}

void Robot_Scan(SDL_Renderer *renderer, Robot *robot, Robot **robots, int nrobots, double d, double w)
{
	double sa = (robot->heading - w / 2) * DEG2RAD;
	double ea = (robot->heading + w / 2) * DEG2RAD;

	if (sa > ea)
	{
		double t = sa;
		sa = ea;
		ea = t;
	}

	double nx1 = d * cos(sa) + robot->x;
	double ny1 = d * sin(sa) + robot->y;
	double nx2 = d * cos(ea) + robot->x;
	double ny2 = d * sin(ea) + robot->y;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	DrawLine(renderer, robot->x, robot->y, nx1, ny1);
	DrawLine(renderer, robot->x, robot->y, nx2, ny2);

	for (double a = sa; a <= ea; a += 0.001)
	{
		double x = d * cos(a) + robot->x;
		double y = d * sin(a) + robot->y;
		DrawPoint(renderer, x, y);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	for (int i = 0; i < nrobots; i++)
	{
		if (robot->id != robots[i]->id)
		{
			if (insector(robot, robots[i], d, w))
			{
				DrawLine(renderer, robot->x, robot->y, robots[i]->x, robots[i]->y);
			}
		}
	}
}

#endif /* ROBOT_IMPLEMENTATION */

#endif /* ROBOT_H */
