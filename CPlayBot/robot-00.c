#define ROBOT_IMPLEMENTATION
#include "robot-00.h"

void start(int id,Robot *robots[],int nrobots) {
    robots[id]->x=rand()%SCREEN_WIDTH;
    robots[id]->y=rand()%SCREEN_HEIGHT;              
}

void update(int id,Robot *robots[],int nrobots) {
    Robot_Move(robots[id],1);

    if(robots[id]->x<=0) robots[id]->heading=WrapAngle(0*DEG2RAD);
    if(robots[id]->y<=0) robots[id]->heading=WrapAngle(90*DEG2RAD);
    if(robots[id]->x>=SCREEN_WIDTH-1) robots[id]->heading=WrapAngle(180*DEG2RAD);
    if(robots[id]->y>=SCREEN_HEIGHT-1) robots[id]->heading=WrapAngle(270*DEG2RAD);

    Robot_Turn(robots[id],(rand()%3-1)*DEG2RAD);
}
