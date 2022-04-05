#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <dlfcn.h>

#include "common.h"

#define ROBOT_IMPLEMENTATION
#include "robot.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;
SDL_Event event;

bool quit = false;

int main(int argc,char **argv)
{
    bool start=true;

    srand(time(NULL));

    if(argc<3) {
        printf("syntax: %s Robot1 Robot2 RobotN...\n",argv[0]);
        return 1;
    }

    int nrobots=argc-1;
    Robot *robots[nrobots];

    for(int i=0; i<nrobots; i++) {

        char name[PATH_MAX];

        robots[i]=Robot_Create(i,argv[i+1],SCREEN_WIDTH/2,SCREEN_HEIGHT/2,rand()%360,4);

        sprintf(name,"./%s.so",argv[i+1]);

        robots[i]->handle = dlopen(name,RTLD_LAZY);

        if (robots[i]->handle == NULL) {
            fprintf(stderr, "Could not open robot %d:%s, %s\n",i,argv[i+1],dlerror());
            return 1;
        }

        robots[i]->start = dlsym(robots[i]->handle, "start");

        if (robots[i]->start == NULL) {
            fprintf(stderr, "Could not find start function of robot %d:%s, %s\n",i,argv[i+1],dlerror());
            return 1;
        }

        robots[i]->update = dlsym(robots[i]->handle, "update");

        if (robots[i]->update == NULL) {
            fprintf(stderr, "Could not find update function of robot %d:%s, %s\n",i,argv[i+1],dlerror());
            return 1;
        }
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderTarget(renderer, display);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        SDL_SetRenderTarget(renderer, display);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &rect);

        if(start) {
            start=false;
            for (int i = 0; i < nrobots; i++) {
                robots[i]->start(robots[i]->id,robots,nrobots);
            }
        }

        for (int i = 0; i < nrobots; i++)
        {
            Robot_Draw(renderer, robots[i]);
            Robot_Scan(renderer, robots[i], robots, nrobots, 200, 90 * DEG2RAD);
        }

        for (int i = 0; i < nrobots; i++) {
            robots[i]->update(robots[i]->id,robots,nrobots);
        }
 
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, display, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
