#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <dlfcn.h>

typedef int start(int id,char *name);

int main() {

    int nrobots=3;

    char *robotNames[]= {"robot-00","robot-00","robot-00"};

    void *hrobot[nrobots];
    start *fstart[nrobots];
    int retval[nrobots];

    int wins[nrobots],nwins=0,highest;

    char name[PATH_MAX];
    
    srand(time(NULL));

    for(int i=0; i<nrobots; i++) {

        sprintf(name,"./%s.so",robotNames[i]);

        hrobot[i] = dlopen(name,RTLD_LAZY);

        if (hrobot[i] == NULL) {
            fprintf(stderr, "Could not open robot %d:%s, %s\n",i,robotNames[i], dlerror());
            return 1;
        }

        fstart[i] = dlsym(hrobot[i], "start");

        if (fstart[i] == NULL) {
            fprintf(stderr, "Could not find start function of robot %d:%s, %s\n",i,robotNames[i],dlerror());
            return 1;
        }



        printf("Running robot %d:%s\n",i,robotNames[i]);

        retval[i] = fstart[i](i,robotNames[i]);

        printf("Robot %d:%s returned %d\n",i,robotNames[i],retval[i]);

        if (dlclose(hrobot[i]) != 0) {
            fprintf(stderr, "Could not close robot %d:%s, %s\n",i,robotNames[i],dlerror());
            return 1;
        }


        
        if(i==0) {
            wins[nwins++]=i;
            highest=retval[i];
        } else if(retval[i]>highest) {
            wins[0]=i;
            nwins=1;
            highest=retval[i];
        } else if(retval[i]==highest) {
            wins[nwins++]=i;
        }

    }

    for(int i=0;i<nwins;i++) {
        printf("Robot %d:%s %s\n",wins[i],robotNames[wins[i]],nwins>1?"draw":"wins");
    }
    
    return 0;
}


