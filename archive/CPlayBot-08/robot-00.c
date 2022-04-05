#define ROBOT_IMPLEMENTATION
#include "robot-00.h"

void start(Robot *robot) {
    robot->x=rand()%SCREEN_WIDTH;
    robot->y=rand()%SCREEN_HEIGHT;              
}

void update(Robot *robot) {
    Robot_Move(robot,1);

    if(robot->x<=0) robot->heading=0*DEG2RAD;
    if(robot->y<=0) robot->heading=90*DEG2RAD;
    if(robot->x>=SCREEN_WIDTH-1) robot->heading=180*DEG2RAD;
    if(robot->y>=SCREEN_HEIGHT-1) robot->heading=270*DEG2RAD;

    Robot_Turn(robot,(rand()%3-1)*DEG2RAD);
}
