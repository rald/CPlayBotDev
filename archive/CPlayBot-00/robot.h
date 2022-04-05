#ifndef ROBOT_H
#define ROB0T_H

#include "common.h"

#define DEG2RAD (M_PI / 180.0)

typedef struct Robot Robot;

struct Robot
{
	double x, y;
	double h;
};

Robot *Robot_Create(double x, double y, double h);
void Robot_Destroy(Robot *robot);
void Robot_Draw(SDL_Renderer *renderer, Robot *robot);
void Robot_Turn(Robot *robot, double a);

#ifdef ROBOT_IMPLEMENTATION



static double ConstrainAngle(double x);



static double ConstrainAngle(double x)
{
	x = fmod(x, 360);
	if (x < 0)
		x += 360;
	return x;
}

Robot *Robot_Create(double x, double y, double h)
{
	Robot *robot = malloc(sizeof(*robot));
	if (robot)
	{
		robot->x = x;
		robot->y = y;
		robot->h = ConstrainAngle(h);
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
		nx[i] = x[i] * cos(robot->h * DEG2RAD) - y[i] * sin(robot->h * DEG2RAD);
		ny[i] = x[i] * sin(robot->h * DEG2RAD) + y[i] * cos(robot->h * DEG2RAD);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i < n - 1; i++)
	{
		DrawLine(renderer, nx[i] + robot->x, ny[i] + robot->y, nx[i + 1] + robot->x, ny[i + 1] + robot->y);
	}
	DrawLine(renderer, nx[n - 1] + robot->x, ny[n - 1] + robot->y, nx[0] + robot->x, ny[0] + robot->y);
}

void Robot_Move(Robot *robot, double d)
{
	robot->x += d * cos(robot->h * DEG2RAD);
	robot->y += d * sin(robot->h * DEG2RAD);
}

void Robot_Turn(Robot *robot, double a)
{
	robot->h = ConstrainAngle(robot->h + a);
}

#endif /* ROBOT_IMPLEMENTATION */

#endif /* ROBOT_H */
