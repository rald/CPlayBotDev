#include "robot-00.h"

int start(int id,char *name) {
    int roll=rnd(6)+1;
    printf("Hello from the robot %d:%s!\n",id,name);
    printf("Robot %d:%s rolls %d\n",id,name,roll);
    return roll;
}
