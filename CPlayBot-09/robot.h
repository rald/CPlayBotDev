#ifndef ROBOT_H
#define ROBOT_H

#include <stdbool.h>
#include <math.h>

#include "common.h"

#define GRAPHICS_IMPLEMENTATION
#include "graphics.h"

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

Robot *Robot_Create(int id, char *name, double x, double y, double heading, double size);
void Robot_Destroy(Robot *robot);
void Robot_Draw(SDL_Renderer *renderer, Robot *robot);
void Robot_Turn(Robot *robot, double a);
void Robot_Scan(SDL_Renderer *renderer, Robot *robot, Robot **robots, int nrobots, double d, double w);

bool inangle(double n,double s,double e,double a);
bool insector(double x, double y, double h, double sx, double sy, double sr, double sw);



#ifdef ROBOT_IMPLEMENTATION

bool inangle(double n,double s,double e,double a) {	
	double b=fmod(a-s,n); b=b<0?b+n:b; 
	return (s<=e)?(b<=e-s):(b<=n-(s-e));
}

bool insector(double x, double y, double cx, double cy, double h, double r, double w)
{
	double sa = WrapAngle(h - w / 2);
	double ea = WrapAngle(h + w / 2);

    double d = distance(cx, cy, x, y);
    double a = WrapAngle(atan2(y - cy, x - cx));

    return inangle(360,sa,ea,a) && d<=r;
}

Robot *Robot_Create(int id, char *name,double x, double y, double heading, double size)
{
	Robot *robot = malloc(sizeof(*robot));
	if (robot)
	{
		robot->id = id;
		robot->name = name;
		robot->x = WrapX(x);
		robot->y = WrapY(y);
		robot->heading = WrapAngle(heading);
		robot->size=size;
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
	double nx[n];
	double ny[n];

	for (int i = 0; i < n; i++)
	{
		nx[i] = (x[i] * cos(robot->heading) - y[i] * sin(robot->heading)) * robot->size + robot->x;
		ny[i] = (x[i] * sin(robot->heading) + y[i] * cos(robot->heading)) * robot->size + robot->y;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    DrawPoly(renderer,nx,ny,n);
}

void Robot_Move(Robot *robot, double distance)
{
	robot->x = WrapX(distance * cos(robot->heading) + robot->x);
	robot->y = WrapY(distance * sin(robot->heading) + robot->y);
}

void Robot_Turn(Robot *robot, double angle)
{
	robot->heading = WrapAngle(robot->heading + angle);
}

void Robot_Scan(SDL_Renderer *renderer, Robot *robot, Robot **robots, int nrobots, double r, double w)
{
	double sa = robot->heading - w / 2;
	double ea = robot->heading + w / 2;

	double nx1 = r * cos(sa) + robot->x;
	double ny1 = r * sin(sa) + robot->y;
	double nx2 = r * cos(ea) + robot->x;
	double ny2 = r * sin(ea) + robot->y;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	DrawLine(renderer, robot->x, robot->y, nx1, ny1);
	DrawLine(renderer, robot->x, robot->y, nx2, ny2);

    double px,py,nx,ny;
    double wa=(ea-sa)/12;
    for(int i = 0; i<=12; i++) {
        if(i==0) {
            px=r*cos(i*wa+sa)+robot->x;
            py=r*sin(i*wa+sa)+robot->y;
        } else {
            nx=r*cos(i*wa+sa)+robot->x;
            ny=r*sin(i*wa+sa)+robot->y;
            DrawLine(renderer,px,py,nx,ny);
            px=nx;
            py=ny;
        }
    }

 	for (int i = 0; i < nrobots; i++)
	{
		if (robot->id != robots[i]->id)
		{
			if (insector(robots[i]->x, robots[i]->y, robot->x, robot->y, robot->heading, r, w))
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				DrawLine(renderer, robot->x, robot->y, robots[i]->x, robots[i]->y);
			}
		}
	}
}



#endif /* ROBOT_IMPLEMENTATION */



#endif /* ROBOT_H */


