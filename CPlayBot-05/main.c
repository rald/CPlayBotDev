#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "common.h"

#define ROBOT_IMPLEMENTATION
#include "robot.h"

#define ROBOT_MAX 10

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;
SDL_Event event;

bool quit = false;

int nrobots = ROBOT_MAX;
Robot *robots[ROBOT_MAX];

int main(void)
{

	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	for (int i = 0; i < nrobots; i++)
	{
		robots[i] = Robot_Create(i, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, rand() % 360, 4);
	}

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

		for (int i = 0; i < nrobots; i++)
		{
			Robot_Draw(renderer, robots[i]);
			Robot_Scan(renderer, robots[i], robots, nrobots, 200, 90 * DEG2RAD);
		}

		for (int i = 0; i < nrobots; i++)
		{
			Robot_Move(robots[i],2);

            if(robots[i]->x<=0) robots[i]->heading=0*DEG2RAD;
            if(robots[i]->y<=0) robots[i]->heading=90*DEG2RAD;
            if(robots[i]->x>=SCREEN_WIDTH-1) robots[i]->heading=180*DEG2RAD;
            if(robots[i]->y>=SCREEN_HEIGHT-1) robots[i]->heading=270*DEG2RAD;

			Robot_Turn(robots[i],(rand()%3-1)*(rand()%8+1)*DEG2RAD);	
        }

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	return 0;
}
