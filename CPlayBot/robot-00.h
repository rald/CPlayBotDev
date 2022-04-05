#ifndef ROBOT_00_H
#define ROBOT_00_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "common.h"

typedef struct Robot Robot;

typedef void fnstart(Robot *robot);
typedef void fnupdate(Robot *robot);

struct Robot
{
	int id;
    char *name;

	double x, y;
	double heading;
    double size;

    void *handle;
    fnstart *start;
    fnupdate *update;
};

void move(Robot *robot, double distance);
void turn(Robot *robot, double angle);

#ifdef ROBOT_IMPLEMENTATION

static double clamp(double d, double min, double max);

static double WrapX(double x);
static double WrapY(double y);
static double WrapAngle(double a);

static double clamp(double d, double min, double max)
{
	double t = d < min ? min : d;
	return t > max ? max : t;
}

static double WrapX(double x)
{
    return clamp(x,0,SCREEN_WIDTH);
}

static double WrapY(double y)
{
    return clamp(y,0,SCREEN_HEIGHT);
}

static double WrapAngle(double a)
{
	a = fmod(a * RAD2DEG, 360);
	if (a < 0)
		a += 360;
	return a * DEG2RAD;
}

void move(Robot *robot, double distance)
{
	robot->x = WrapX(distance * cos(robot->heading) + robot->x);
	robot->y = WrapY(distance * sin(robot->heading) + robot->y);
}

void turn(Robot *robot, double angle)
{
	robot->heading = WrapAngle(robot->heading + angle);
}

#endif /* ROBOT_IMPLEMENTATION */

#endif /* ROBOT_H */
